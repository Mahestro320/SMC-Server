/*
 * SMC Server (Super Mega Cool Server)
 * created the 18/06/25
 *
 * by Mahestro_320
 *
 * boost version: 1.88.0
 */

#include <array>
#include <boost/asio.hpp>
#include "common.hpp"
#include "io/console.hpp"
#include "io/logger.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/config.hpp"
#include "users/manager.hpp"
#include "util/thread.hpp"

using boost::asio::ip::tcp;

namespace {

void init() {
    common::main_thread_id = util::thread::getCurrentThreadIdAsString();
    Logger& logger{Logger::getInstance()};
    logger.init();
    UsersManager& users_manager{UsersManager::getInstance()};
    users_manager.load();
}

void handleClient(tcp::socket socket) {
    try {
        Client client{std::move(socket)};
        client.start();
    } catch (const std::bad_alloc& e) {
        console::out::err("memory error: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err("unhandled exception: " + std::string{e.what()});
    }
    console::out::inf("thread stopped");
}

void buildSocketOnConnection(boost::asio::io_context& io_context, tcp::acceptor& acceptor) {
    tcp::socket socket{io_context};
    acceptor.accept(socket);
    std::thread t{handleClient, std::move(socket)};
    t.detach();
}

void runServer() {
    boost::asio::io_context io_context{};
    tcp::acceptor acceptor{io_context, tcp::endpoint(tcp::v4(), network::config::PORT)};
    console::out::inf("server started, waiting for connections");
    while (common::is_running) {
        try {
            buildSocketOnConnection(io_context, acceptor);
        } catch (const std::bad_alloc& e) {
            console::out::err("memory error: " + std::string{e.what()});
        } catch (const std::exception& e) {
            console::out::err("connect error: " + std::string{e.what()});
        }
    }
}

} // namespace

int main(int /* argc */, char** /* argv */) {
    init();
    console::out::inf("starting server v" + common::VERSION.toString() + " (" + network::getIpv4Address() + ":" +
                      std::to_string(network::config::PORT) + ")");
    try {
        runServer();
    } catch (const std::bad_alloc& e) {
        console::out::err("UNHANDLED MEMORY ERROR: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err("UNHANDLED EXCEPTION: " + std::string(e.what()));
    }
    return 0;
}
