#include "io/console.hpp"
#include "io/directories.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/every_handlers.hpp"
#include "network/response/id.hpp"
#include "util/string.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

Client::Client(tcp::socket socket) : socket{std::move(socket)} {
}

void Client::start() {
    printLocation();
    try {
        startCommunication();
    } catch (std::exception& e) {
        console::out::err(e);
        is_connected = false;
    }
}

void Client::printLocation() const {
    const tcp::endpoint remote_endpoint{socket.remote_endpoint()};
    const boost::asio::ip::address client_address{remote_endpoint.address()};
    console::out::inf("connected client at " + client_address.to_string() + ":" +
                      std::to_string(remote_endpoint.port()));
}

void Client::startCommunication() {
    sendWelcomeMessage();
    while (is_connected) {
        handleNewRequest();
    }
}

void Client::sendWelcomeMessage() {
    boost::asio::write(socket, boost::asio::buffer(WELCOME_MESSAGE));
}

void Client::handleNewRequest() {
    boost::system::error_code error_code{};
    std::uint8_t request{};
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(&request, 1), error_code)};
    if (error_code || bytes_read == 0) {
        handleReadError(error_code);
        return;
    }
    RequestId request_id{static_cast<RequestId>(request)};
    console::out::inf("received request: " + std::to_string(request) + " (" + network::request::getName(request_id) +
                      ")");
    try {
        processReceivedRequest(request_id);
    } catch (const std::exception& e) {
        console::out::err(e);
    }
}

void Client::handleReadError(boost::system::error_code error_code) {
    is_connected = false;
    if (error_code == boost::asio::error::eof) {
        console::out::inf("client disconnected");
    } else {
        console::out::err("client disconnected: network error");
    }
}

void Client::processReceivedRequest(RequestId request) {
    RH* handler{getHandlerFromRequest(request)};
    if (!handler) {
        console::out::err("unknown request: " + std::to_string(static_cast<uint8_t>(request)));
        network::sendResponse(socket, ResponseId::UnknownRequest);
        return;
    }
    handler->setClient(this);
    handler->run();
    delete handler;
}

RH* Client::getHandlerFromRequest(RequestId request) const {
    if (request == RequestId::Login) {
        return new LoginRH{};
    } else if (request == RequestId::Logout) {
        return new LogoutRH{};
    } else if (request == RequestId::GetUserRole) {
        return new GetUserRoleRH{};
    } else if (request == RequestId::GetDirContent) {
        return new GetDirContentRH{};
    } else if (request == RequestId::IOFileExists) {
        return new IOFileExistsRH{};
    } else if (request == RequestId::IOGetFileType) {
        return new IOGetFileTypeRH{};
    } else if (request == RequestId::IOGetRealPath) {
        return new IOGetRealPathRH{};
    }
    return nullptr;
}

bool Client::isLogged() const {
    return is_logged;
}

tcp::socket& Client::getSocket() {
    return socket;
}

const User& Client::getUser() const {
    return user;
}

SFS& Client::getSFS() {
    return sfs;
}

void Client::setUser(const User& user) {
    this->user = user;
}

void Client::setLogged(bool is_logged) {
    this->is_logged = is_logged;
}
