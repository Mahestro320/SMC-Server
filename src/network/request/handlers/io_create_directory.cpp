#include "network/request/handlers/io_create_directory.hpp"

#include <fstream>
#include "network.hpp"
#include "network/client.hpp"
#include "util/error_message.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IOCreateDirectoryRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    if (!readPath(socket) || !checkPath() || !createDirectory()) {
        network::sendResponse(socket, response);
        return;
    }
    network::sendResponse(socket, ResponseId::Ok);
}

bool IOCreateDirectoryRH::readPath(tcp::socket& socket) {
    std::string str{};
    if (!network::readString(socket, str)) {
        response = ResponseId::InvalidData;
        return false;
    }
    const SFS& sfs{client->getSFS()};
    path = sfs.absoluteFromUserScope(fs::path{str});
    return true;
}

bool IOCreateDirectoryRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    if (sfs.exists(path)) {
        response = ResponseId::IOAlreadyExists;
    } else if (!sfs.canLexicallyAccess(path)) {
        response = ResponseId::InvalidPermissions;
    } else {
        return true;
    }
    return false;
}

bool IOCreateDirectoryRH::createDirectory() {
    if (!fs::create_directory(path)) {
        console::out::err("error while creating directory \"" + path.string() + '"');
        return false;
    }
    return true;
}
