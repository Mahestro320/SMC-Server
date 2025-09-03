/*
 * SMC Server (Super Mega Cool Server)
 * created the 18/06/25 (DD/MM/YY)
 *
 * by Mahestro_320
 */

#include "system/beg.hpp"

#include <array>
#include <atomic>
#include <boost/asio.hpp>
#include "common.hpp"
#include "io/console.hpp"
#include "io/logger.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "server/config.hpp"
#include "users/manager.hpp"
#include "util/thread.hpp"

using boost::asio::ip::tcp;

namespace {

std::atomic<uint32_t> client_count{};

void init() {
    common::main_thread_id = util::thread::getCurrentThreadIdAsString();
    Config& config{Config::getInstance()};
    config.load();
    Logger& logger{Logger::getInstance()};
    logger.init();
    console::init();
    UsersManager& users_manager{UsersManager::getInstance()};
    users_manager.load();
}

void handleClient(tcp::socket socket) {
    try {
        const uint32_t client_idx{client_count.load(std::memory_order_acquire)};
        client_count.fetch_add(1, std::memory_order_release);
        Client::setInstance(std::move(socket), client_idx);
        Client& client{Client::getInstance()};
        client.start();
    } catch (const std::bad_alloc& e) {
        console::out::err("memory error: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err("unhandled exception: " + std::string{e.what()});
    }
    console::out::inf("thread stopped");
}

void acceptConnections(tcp::acceptor& acceptor, boost::asio::io_context& io_context) {
    acceptor.async_accept([&](const boost::system::error_code& ec, tcp::socket socket) {
        if (ec) {
            console::out::err("error while accepting connection: " + ec.message());
        } else {
            std::thread t{handleClient, std::move(socket)};
            t.detach();
        }
        acceptConnections(acceptor, io_context);
    });
}

void runServer(uint16_t port) {
    boost::asio::io_context io_context{};
    tcp::endpoint endpoint{tcp::v4(), port};
    tcp::acceptor acceptor{io_context};

    acceptor.open(endpoint.protocol());
    acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen();

    console::out::inf("server started, waiting for connections");
    acceptConnections(acceptor, io_context);
    io_context.run();
}

} // namespace

int main(int /* argc */, char** /* argv */) {
    ::init();
    Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    console::out::inf("starting server <:color=green>v<$version><:color=reset> "
                      "(<:color=bright_blue><$address>:<$port><:color=reset>)");
    console::out::inf("time format: <:color=bright_blue><$time_format>");
    try {
        ::runServer(config_values.server_port);
    } catch (const std::bad_alloc& e) {
        console::out::err("UNHANDLED MEMORY ERROR: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err("UNHANDLED EXCEPTION: " + std::string(e.what()));
    }
    return 0;
}
