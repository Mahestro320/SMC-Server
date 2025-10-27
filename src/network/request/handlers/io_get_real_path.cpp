#include "network/request/handlers/io_get_real_path.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IOGetRealPathRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!client->isLogged()) {
        console::out::err("the client isn't logged");
        network::sendResponse(socket, ResponseId::NotLogged);
        return;
    }
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    getInputPath(socket) && checkInputPath(socket) && setRealPath() && sendRealPath(socket);
}

bool IOGetRealPathRH::getInputPath(tcp::socket& socket) {
    std::string path_str{};
    console::out::inf("reading input path");
    if (!network::readString(socket, path_str)) {
        return false;
    }
    const SFS& sfs{client->getSFS()};
    input_path = sfs.absoluteFromUserScope(fs::path{path_str});
    return true;
}

bool IOGetRealPathRH::checkInputPath(tcp::socket& socket) const {
    const SFS& sfs{client->getSFS()};
    if (!sfs.exists(input_path)) {
        network::sendResponse(socket, ResponseId::IOFileNotExists);
        console::out::inf("the input path does not exists");
        return false;
    }
    console::out::inf("the input path exists");
    network::sendResponse(socket, ResponseId::Ok);
    return true;
}

bool IOGetRealPathRH::setRealPath() {
    const SFS& sfs{client->getSFS()};
    console::out::inf("real path: " + input_path.string());
    real_path = sfs.real(input_path);
    return true;
}

bool IOGetRealPathRH::sendRealPath(tcp::socket& socket) const {
    console::out::inf("sending real path");
    const SFS& sfs{client->getSFS()};
    const fs::path relative_real_path{sfs.relativeToUserScope(real_path)};
    return network::sendString(socket, relative_real_path.generic_string());
}
