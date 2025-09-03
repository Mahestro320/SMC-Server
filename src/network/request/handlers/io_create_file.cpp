#include "network/request/handlers/io_create_file.hpp"

#include <fstream>
#include "network.hpp"
#include "network/client.hpp"
#include "util/error_message.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IOCreateFileRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    if (!readPath(socket) || !checkPath() || !createFile()) {
        network::sendResponse(socket, response);
        return;
    }
    network::sendResponse(socket, ResponseId::Ok);
}

bool IOCreateFileRH::readPath(tcp::socket& socket) {
    std::string str{};
    if (!network::readString(socket, str)) {
        response = ResponseId::InvalidData;
        return false;
    }
    const SFS& sfs{client->getSFS()};
    path = sfs.absoluteFromUserScope(fs::path{str});
    return true;
}

bool IOCreateFileRH::checkPath() {
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

bool IOCreateFileRH::createFile() {
    fs::create_directories(path.parent_path());
    std::ofstream file{path};
    if (!file.is_open()) {
        console::out::err("error while creating file \"" + path.string() + "\": " + util::error_message::get(errno));
        return false;
    }
    return true;
}
