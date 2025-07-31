#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/get_user_role.hpp"

using boost::asio::ip::tcp;

void GetUserRoleRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    if (!client->isLogged()) {
        console::out::inf("the user isn't logged");
        network::sendResponse(socket, ResponseId::NotLogged);
        return;
    }
    sendUserRole(socket);
}

void GetUserRoleRH::sendUserRole(tcp::socket& socket) {
    const User& user{client->getUser()};
    const uint8_t user_role{static_cast<uint8_t>(user.role)};
    console::out::inf("sending user role: " + std::to_string(user_role) + " (" + role::getName(user.role) + ")");
    network::sendResponse(socket, ResponseId::Ok);
    network::sendInt(socket, user_role);
}
