#include "network/request/handlers/logout.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

void LogoutRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!sendBeginResponse(socket)) {
        return;
    }
    client->setLogged(false);
    client->setUser({});
    network::sendResponse(socket, ResponseId::Ok);
}

bool LogoutRH::sendBeginResponse(tcp::socket& socket) {
    if (!client->isLogged()) {
        console::out::inf("the client isn't logged");
        network::sendResponse(socket, ResponseId::NotLogged);
        return false;
    }
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return false;
    }
    return true;
}
