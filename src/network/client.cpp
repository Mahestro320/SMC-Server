#include "network/client.hpp"

#include "io/console.hpp"
#include "io/directories.hpp"
#include "network.hpp"
#include "network/request/handlers/get_user_role.hpp"
#include "network/request/handlers/io_create_directory.hpp"
#include "network/request/handlers/io_create_file.hpp"
#include "network/request/handlers/io_file_exists.hpp"
#include "network/request/handlers/io_get_dir_content.hpp"
#include "network/request/handlers/io_get_file_content.hpp"
#include "network/request/handlers/io_get_file_size.hpp"
#include "network/request/handlers/io_get_file_type.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "network/request/handlers/io_set_file_content.hpp"
#include "network/request/handlers/login.hpp"
#include "network/request/handlers/logout.hpp"
#include "network/request/handlers/run_syscmd.hpp"
#include "network/response/id.hpp"
#include "util/string.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

thread_local std::unique_ptr<Client> Client::thread_instance{nullptr};

Client::Client(tcp::socket socket, uint32_t index) : socket{std::move(socket)}, index{index} {
}

void Client::setInstance(boost::asio::ip::tcp::socket socket, uint32_t index) {
    if (thread_instance != nullptr) {
        throw std::logic_error("Client instance already set for this thread");
    }
    thread_instance = std::make_unique<Client>(std::move(socket), index);
}

Client& Client::getInstance() {
    if (thread_instance == nullptr) {
        throw std::logic_error("Client instance not set for this thread");
    }
    return *thread_instance;
}

bool Client::isInstanceSet() {
    return thread_instance != nullptr;
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
    console::out::inf("connected client at <:color=bright_blue>" + getAddress() + ':' + std::to_string(getPort()) +
                      "<:color=reset>");
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
    console::out::inf("received request: <:color=bright_blue>" + std::to_string(request) +
                      "<:color=reset> (<:color=green>" + network::request::getName(request_id) + "<:color=reset>)");
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
    } else if (request == RequestId::IOGetDirContent) {
        return new IOGetDirContentRH{};
    } else if (request == RequestId::IOFileExists) {
        return new IOFileExistsRH{};
    } else if (request == RequestId::IOGetFileType) {
        return new IOGetFileTypeRH{};
    } else if (request == RequestId::IOGetFileSize) {
        return new IOGetFileSizeRH{};
    } else if (request == RequestId::IOGetFileContent) {
        return new IOGetFileContentRH{};
    } else if (request == RequestId::IOGetRealPath) {
        return new IOGetRealPathRH{};
    } else if (request == RequestId::IOSetFileContent) {
        return new IOSetFileContentRH{};
    } else if (request == RequestId::IOCreateFile) {
        return new IOCreateFileRH{};
    } else if (request == RequestId::IOCreateDirectory) {
        return new IOCreateDirectoryRH{};
    } else if (request == RequestId::RunSyscmd) {
        return new RunSyscmdRH{};
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

uint32_t Client::getIndex() const {
    return index;
}

uint16_t Client::getPort() const {
    const tcp::endpoint remote_endpoint{getRemoteEndpoint()};
    const boost::asio::ip::port_type port{remote_endpoint.port()};
    return port;
}

std::string Client::getAddress() const {
    const tcp::endpoint remote_endpoint{getRemoteEndpoint()};
    const boost::asio::ip::address address{remote_endpoint.address()};
    return address.to_string();
}

tcp::endpoint Client::getRemoteEndpoint() const {
    return socket.remote_endpoint();
}

void Client::setUser(const User& user) {
    this->user = user;
}

void Client::setLogged(bool is_logged) {
    this->is_logged = is_logged;
}
