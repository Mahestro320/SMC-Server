#include "network/request/handlers/login.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "users/manager.hpp"

using boost::asio::ip::tcp;

void LoginRH::run() {
    tcp::socket& socket{client->getSocket()};
    network::sendResponse(socket, ResponseId::Ok) && network::sendResponse(socket, logClientFrom(socket));
}

ResponseId LoginRH::logClientFrom(tcp::socket& socket) {
    if (!getUsername(socket) || !getPassword(socket)) {
        return ResponseId::InvalidData;
    }
    if (!getUserFromUsername()) {
        return ResponseId::UnknownUsername;
    }
    if (!checkPassword()) {
        return ResponseId::InvalidPassword;
    }
    client->setUser(user);
    client->setLogged(true);
    return ResponseId::Ok;
}

bool LoginRH::getUsername(tcp::socket& socket) {
    console::out::inf("getting username");
    return network::readString(socket, username);
}

bool LoginRH::getPassword(tcp::socket& socket) {
    console::out::inf("getting password");
    return network::readString(socket, password);
}

bool LoginRH::getUserFromUsername() {
    console::out::inf("getting user");
    UsersManager& users_manager{UsersManager::getInstance()};
    user = users_manager.get(username);
    if (!user.is_valid) {
        console::out::err("the user does not exists");
        return false;
    }
    return true;
}

bool LoginRH::checkPassword() {
    console::out::inf("checking password");
    UsersManager& users_manager{UsersManager::getInstance()};
    if (password == user.password) {
        user.wrong_password_count = 0;
        return users_manager.set(user);
    }
    console::out::warn("invalid password !");
    const uint64_t wait_time_in_seconds{static_cast<uint64_t>(std::pow(user.wrong_password_count, 2))};
    if (wait_time_in_seconds != 0) {
        console::out::inf("waiting " + std::to_string(wait_time_in_seconds) + " seconds");
        std::this_thread::sleep_for(std::chrono::seconds{wait_time_in_seconds});
    }
    ++user.wrong_password_count;
    users_manager.set(user);
    return false;
}
