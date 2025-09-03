#include "io/console.hpp"

#include <condition_variable>
#include <deque>
#include <iostream>
#include <shared_mutex>
#include "io/logger.hpp"
#include "network/client.hpp"
#include "termcolor.hpp"
#include "util/resolver/con_str_prop.hpp"
#include "util/resolver/evs_str.hpp"
#include "util/time.hpp"

/*
 * message format:
 * [<message type> @clt-<client count> | <date: DD/MM/YYYY> <time: HH:MM:SS>] <message>
 *
 * for example:
 * [INFO @clt-221 | 10/09/25 4:33:17] hello !
 *
 * Messages can contains colors, environnement vars and sysvars.
 * To set the color:
 * <:color=[COLOR]>
 *
 */

namespace {

enum class MessageType : uint8_t {
    Info,
    Warning,
    Error
};

struct MessageInfo final {
    MessageType type{};
    std::string message{};
    uint64_t timestamp{};
    std::ostream* stream{&std::cout};
};

std::mutex queue_mutex{};
std::deque<MessageInfo> queue{};
std::condition_variable cv{};

void changeColorAccordingToType(MessageType type, std::ostream& stream) {
    if (type == MessageType::Info) {
        stream << termcolor::green;
    } else if (type == MessageType::Warning) {
        stream << termcolor::yellow;
    } else if (type == MessageType::Error) {
        stream << termcolor::red;
    } else {
        stream << termcolor::white;
    }
}

std::string messageTypeToString(MessageType type) {
    if (type == MessageType::Info) {
        return "INFO";
    } else if (type == MessageType::Warning) {
        return "WARNING";
    } else if (type == MessageType::Error) {
        return "ERROR";
    }
    return "UNKNOWN";
}

std::string getMessageLocation() {
    if (!Client::isInstanceSet()) {
        return "@Main";
    }
    Client& client{Client::getInstance()};
    return "@clt-" + std::to_string(client.getIndex());
}

void printMessageContent(const std::string& message, std::ostream* stream) {
    ConStrPropResolver resolver{};
    resolver.setStream(stream);
    resolver.setText(message);
    resolver.print();
}

void printMessage(MessageInfo msg_info) {
    *msg_info.stream << termcolor::bright_white << '[';
    changeColorAccordingToType(msg_info.type, *msg_info.stream);
    *msg_info.stream << messageTypeToString(msg_info.type) << termcolor::blue << ' ' << getMessageLocation()
                     << termcolor::cyan << " | " << termcolor::bright_green
                     << util::time::formatTime(msg_info.timestamp) << termcolor::bright_white << "] "
                     << termcolor::white;
    printMessageContent(msg_info.message, msg_info.stream);
    *msg_info.stream << std::endl;
}

void logMessage(MessageType type, const std::string& message, uint64_t timestamp) {
    Logger& logger{Logger::getInstance()};
    const std::string formated_message{'[' + messageTypeToString(type) + ' ' + getMessageLocation() + " | " +
                                       util::time::formatTime(timestamp) + "] " + message + '\n'};
    ConStrPropResolver resolver{};
    resolver.setStream(&logger.getStream());
    resolver.setText(formated_message);
    resolver.print();
}

std::string getResolvedMessage(const std::string& input) {
    EVSStrResolver<false> resolver{};
    resolver.setInput(input);
    resolver.resolve();
    return resolver.getOutput();
}

void processMessage(MessageInfo msg_info) {
    {
        std::lock_guard lock(queue_mutex);
        queue.push_back(std::move(msg_info));
    }
    cv.notify_one();
}

void printerThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cv.wait(lock, [] { return !queue.empty(); });
        MessageInfo msg_info{std::move(queue.front())};
        queue.pop_front();
        lock.unlock();
        MessageInfo resolved_msg_info{msg_info};
        resolved_msg_info.message = getResolvedMessage(msg_info.message);
        printMessage(resolved_msg_info);
        logMessage(msg_info.type, resolved_msg_info.message, msg_info.timestamp);
    }
}

} // namespace

void console::init() {
    std::thread t{printerThread};
    t.detach();
}

void console::out::inf(const std::string& message) {
    processMessage(MessageInfo{
        .type = MessageType::Info,
        .message = message,
        .timestamp = util::time::getSeconds(),
    });
}

void console::out::warn(const std::string& message) {
    processMessage(MessageInfo{
        .type = MessageType::Warning,
        .message = "<:color=yellow>" + message,
        .timestamp = util::time::getSeconds(),
    });
}

void console::out::err(const std::string& message) {
    processMessage(MessageInfo{
        .type = MessageType::Error,
        .message = "<:color=red>" + message,
        .timestamp = util::time::getSeconds(),
        .stream = &std::cerr,
    });
}

void console::out::err(const std::exception& e) {
    console::out::err(e.what());
}
