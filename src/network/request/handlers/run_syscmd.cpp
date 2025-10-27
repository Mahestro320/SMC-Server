#include "network/request/handlers/run_syscmd.hpp"

#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

void RunSyscmdRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!sendBeginResponse(socket) || !readCommand(socket)) {
        return;
    }
    runCommand();
    sendOutput(socket);
}

bool RunSyscmdRH::sendBeginResponse(tcp::socket& socket) const {
    if (!client->isLogged()) {
        console::out::err("the client isn't logged");
        network::sendResponse(socket, ResponseId::NotLogged);
        return false;
    }
    const User& user{client->getUser()};
    if (user.role < Role::Developer) {
        console::out::err("invalid permissions");
        network::sendResponse(socket, ResponseId::InvalidPermissions);
        return false;
    }
    return network::sendResponse(socket, ResponseId::Ok);
}

bool RunSyscmdRH::readCommand(tcp::socket& socket) {
    return network::readString(socket, command);
}

void RunSyscmdRH::runCommand() {
    command += " 2>&1"; // redirects stderr to stdout
    FILE* pipe{_popen(command.data(), "r")};
    if (!pipe) {
        output = "error while opening pipe";
        return;
    }
    char buffer[256]{};
    while (fgets(buffer, 256, pipe)) {
        output += buffer;
    }
    const int code{_pclose(pipe)};
    if (code != 0) {
        output += "\nreturn code: " + std::to_string(code);
    }
}

bool RunSyscmdRH::sendOutput(tcp::socket& socket) const {
    return network::sendString(socket, output);
}
