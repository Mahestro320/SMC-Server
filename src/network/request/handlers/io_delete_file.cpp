#include "network/request/handlers/io_delete_file.hpp"

#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IODeleteFileRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    if (!readPath(socket) || !checkPath()) {
        network::sendResponse(socket, response);
        return;
    }
    deleteFile();
    network::sendResponse(socket, ResponseId::Ok);
}

bool IODeleteFileRH::readPath(tcp::socket& socket) {
    std::string str{};
    if (!network::readString(socket, str)) {
        response = ResponseId::InvalidData;
        return false;
    }
    const SFS& sfs{client->getSFS()};
    path = sfs.absoluteFromUserScope(fs::path{str});
    return true;
}

bool IODeleteFileRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    if (!sfs.canLexicallyAccess(path)) {
        response = ResponseId::InvalidPermissions;
    } else if (!sfs.exists(path)) {
        response = ResponseId::IOFileNotExists;
    } else {
        return true;
    }
    return false;
}

bool IODeleteFileRH::deleteFile() {
    std::error_code ec{};
    uintmax_t elems_removed{fs::remove_all(path, ec)};
    if (ec) {
        console::out::err("error while removing file: " + ec.message());
        response = ResponseId::IOError;
        return false;
    }
    console::out::inf("successfully removed " + std::to_string(elems_removed) + " elems");
    return true;
}
