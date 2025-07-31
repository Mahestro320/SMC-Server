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
#include "server/config.hpp"
#include "users/manager.hpp"
#include "util/thread.hpp"

#include "io/directories.hpp" // DEBUG

using boost::asio::ip::tcp;

namespace {

void init() {
    common::main_thread_id = util::thread::getCurrentThreadIdAsString();
    Config& config{Config::getInstance()};
    config.load();
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
    const ConfigValues& values{config.getValues()};
    console::out::inf("starting server v" + common::VERSION.toString() + " (" + network::getIpv4Address() + ":" +
                      std::to_string(values.server_port) + ")");
    try {
        ::runServer(values.server_port);
    } catch (const std::bad_alloc& e) {
        console::out::err("UNHANDLED MEMORY ERROR: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err("UNHANDLED EXCEPTION: " + std::string(e.what()));
    }
    return 0;
}
