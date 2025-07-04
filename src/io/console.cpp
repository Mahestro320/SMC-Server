#include <filesystem>
#include <iostream>
#include <mutex>
#include <termcolor.hpp>
#include "common.hpp"
#include "io/console.hpp"
#include "io/directories.hpp"
#include "io/logger.hpp"
#include "util/string.hpp"
#include "util/thread.hpp"

namespace fs = std::filesystem;

namespace {

enum class MessagePrefixColor {
    WHITE,
    GREEN,
    YELLOW,
    RED,
    BLUE
};

struct Message {
    const std::string type{}, content{}, location{};
    const MessagePrefixColor color{};
    const bool is_error{false};
};

std::string makeRelative(std::string path) {
    std::replace(path.begin(), path.end(), '/', '\\');

    const std::string root_str{dirs::COMPILATION_ROOT.string()};
    auto pos{path.find(root_str)};
    if (pos != std::string::npos) {
        return path.substr(pos + root_str.size() + 1);
    }
    return path;
}

std::string getThreadMessage() {
    const std::string current_thread_id_str{util::thread::getCurrentThreadIdAsString()};
    return current_thread_id_str == common::main_thread_id ? "" : " (thread " + current_thread_id_str + ')';
}

std::string getMessageLocation(const std::source_location& location) {
    std::string msg_path_rel_str{};
    try {
        const fs::path msg_path_with_ext{location.file_name()};
        const fs::path msg_path{msg_path_with_ext.parent_path() / msg_path_with_ext.stem()};
        msg_path_rel_str = makeRelative(msg_path.string());
    } catch (...) {
        msg_path_rel_str = "<UNKNOWN LOCATION>";
    }
    return getThreadMessage() + " @" + msg_path_rel_str + ':' + std::to_string(location.line());
}

void writeColorInStream(std::ostream& stream, MessagePrefixColor color) {
    switch (color) {
    case MessagePrefixColor::WHITE:
        stream << termcolor::white;
        break;

    case MessagePrefixColor::GREEN:
        stream << termcolor::green;
        break;

    case MessagePrefixColor::YELLOW:
        stream << termcolor::yellow;
        break;

    case MessagePrefixColor::RED:
        stream << termcolor::red;
        break;

    case MessagePrefixColor::BLUE:
        stream << termcolor::blue;
        break;
    }
}

void writeMessageInLog(const Message& msg) {
    const Logger& logger{Logger::getInstance()};
    logger.log('[' + msg.type + msg.location + "] " + msg.content + '\n');
}

void printLineFormatted(const Message& msg) {
    static std::mutex mutex;
    std::lock_guard<std::mutex> guard(mutex);
    writeMessageInLog(msg);
    std::ostream& stream{msg.is_error ? std::cerr : std::cout};
    stream << termcolor::bright_white << '[';
    writeColorInStream(stream, msg.color);
    stream << msg.type << termcolor::bright_blue << msg.location << termcolor::bright_white << "] " << termcolor::reset
           << msg.content << '\n';
    return;
}

} // namespace

#ifdef DEBUG
void console::out::ptr(const void* ptr, const std::source_location& location) {
    printLineFormatted(Message{
        .type = "POINTER",
        .content = util::string::ptrToString(ptr),
        .location = getMessageLocation(location),
        .color = MessagePrefixColor::WHITE,
    });
}

void console::out::debug(const std::string& inf, const std::source_location& location) {
    printLineFormatted(Message{
        .type = "DEBUG",
        .content = inf,
        .location = getMessageLocation(location),
        .color = MessagePrefixColor::BLUE,
    });
}
#endif

void console::out::inf(const std::string& inf, const std::source_location& location) {
    printLineFormatted(Message{
        .type = "INFO",
        .content = inf,
        .location = getMessageLocation(location),
        .color = MessagePrefixColor::GREEN,
    });
}

void console::out::warn(const std::string& warn, const std::source_location& location) {
    printLineFormatted(Message{
        .type = "WARNING",
        .content = warn,
        .location = getMessageLocation(location),
        .color = MessagePrefixColor::YELLOW,
    });
}

void console::out::err(const std::string& err, const std::source_location& location) {
    printLineFormatted(Message{
        .type = "ERROR",
        .content = err,
        .location = getMessageLocation(location),
        .color = MessagePrefixColor::YELLOW,
        .is_error = true,
    });
}

void console::out::err(const std::exception& e, const std::source_location& location) {
    console::out::err(e.what(), location);
}
