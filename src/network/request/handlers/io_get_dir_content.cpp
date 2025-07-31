#include "io/console.hpp"
#include "network.hpp"
#include "network/bfl/encoder.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_dir_content.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void IOGetDirContentRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!sendBeginResponse(socket)) {
        return;
    }
    try {
        getFileList();
    } catch (const std::exception& e) {
        console::out::err(e);
    }
    sendBuffer(socket);
    console::out::inf("done");
}

bool IOGetDirContentRH::sendBeginResponse(tcp::socket& socket) {
    if (!client->isLogged()) {
        console::out::err("the client must be logged to do this");
        network::sendResponse(socket, ResponseId::NotLogged);
        return false;
    }
    if (!network::sendResponse(socket, ResponseId::Ok) || !getPath(socket)) {
        return false;
    }
    return true;
}

bool IOGetDirContentRH::getPath(tcp::socket& socket) {
    console::out::inf("getting the path to list");
    std::string path_string{};
    if (!network::readString(socket, path_string)) {
        return false;
    }
    const SFS& sfs{client->getSFS()};
    path = sfs.absoluteFromUserScope(fs::path{path_string});
    ResponseId response_id{};
    try {
        response_id = checkPath();
    } catch (const std::exception& e) {
        console::out::err("invalid path: " + std::string{e.what()});
        response_id = ResponseId::IOInvalidPath;
    }
    return network::sendResponse(socket, response_id) && response_id == ResponseId::Ok;
}

ResponseId IOGetDirContentRH::checkPath() const {
    if (!fs::exists(path)) {
        console::out::err("the path does not exists");
        return ResponseId::IOFileNotExists;
    }
    if (!fs::is_directory(path)) {
        console::out::err("the path isn't a directory");
        return ResponseId::IONotDirectory;
    }
    return ResponseId::Ok;
}

void IOGetDirContentRH::getFileList() {
    const SFS& sfs{client->getSFS()};
    for (const fs::path& file : sfs.getDirectoryFileList(path)) {
        try {
            pushFileInList(file);
        } catch (const std::exception& e) {
            console::out::err(e);
        }
    }
}

void IOGetDirContentRH::pushFileInList(const fs::path& file) {
    const std::filesystem::path& name_path{file.filename()};
    FileInfo file_info{
        .name = name_path.string(),
        .type = FileType::None,
    };
    if (fs::is_regular_file(file)) {
        file_info.type = FileType::File;
        file_info.size = static_cast<size_t>(fs::file_size(file));
    } else if (fs::is_directory(file)) {
        file_info.type = FileType::Directory;
    }
    data.push_back(file_info);
}

bool IOGetDirContentRH::sendBuffer(tcp::socket& socket) const {
    BFLEncoder encoder{};
    encoder.setData(data);
    encoder.encode();
    console::out::inf("sending BFL");
    return network::sendBuffer(socket, encoder.getBFL());
}
