#include "io/console.hpp"
#include "network.hpp"
#include "network/request/handlers/logout.hpp"

using boost::asio::ip::tcp;

void LogoutRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!client->isLogged()) {
        console::out::inf("the client must be connected to do this");
        network::sendResponse(socket, ResponseId::NotLogged);
        return;
    }
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    client->setLogged(false);
    client->setUser({});
    network::sendResponse(socket, ResponseId::Ok);
}
