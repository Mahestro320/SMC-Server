#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_file_content.hpp"

namespace fs = std::filesystem;

void IOGetFileContentRH::run() {
    socket = &client->getSocket();
    if (!init()) {
        return;
    }
    getFileSize();
    calcBufferCount();
    if (!tryToOpenFile()) {
        return;
    }
    try {
        startTransmition();
    } catch (const std::exception& e) {
        console::out::err(e);
    }
    closeFile();
}

bool IOGetFileContentRH::init() {
    return network::sendResponse(*socket, ResponseId::Ok) && readBufferSize() && checkBufferSize() && readPath() &&
           checkPath();
}

void IOGetFileContentRH::getFileSize() {
    const SFS& sfs{client->getSFS()};
    file_size = static_cast<size_t>(sfs.getFileSize(complete_path));
}

bool IOGetFileContentRH::tryToOpenFile() {
    bool success{};
    try {
        success = openFile();
        network::sendResponse(*socket, response);
    } catch (const std::exception& e) {
        console::out::err(e);
        return false;
    }
    return success;
}

void IOGetFileContentRH::calcBufferCount() {
    buffer_count = static_cast<size_t>(std::ceil(static_cast<double>(file_size) / buffer_size));
    console::out::inf("buffer count: " + std::to_string(buffer_count));
}

bool IOGetFileContentRH::readBufferSize() {
#if SIZE_MAX == UINT64_MAX
    return network::readInt<uint64_t>(*socket, buffer_size);
#else
    uint64_t buffer_size_64{};
    if (!network::readInt<uint64_t>(*socket, buffer_size_64)) {
        return false;
    }
    buffer_size = static_cast<size_t>(buffer_size_64);
    return true;
#endif
}

bool IOGetFileContentRH::checkBufferSize() {
    if (buffer_size == 0 || buffer_size > MAX_BUFFER_SIZE) {
        network::sendResponse(*socket, ResponseId::InvalidBufferSize);
        return false;
    }
    network::sendResponse(*socket, ResponseId::Ok);
    return true;
}

bool IOGetFileContentRH::readPath() {
    std::string path_str{};
    if (!network::readString(*socket, path_str)) {
        return false;
    }
    base_path = fs::path{path_str};
    const SFS& sfs{client->getSFS()};
    complete_path = sfs.absoluteFromUserScope(base_path);
    return true;
}

bool IOGetFileContentRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    if (!sfs.exists(complete_path)) {
        console::out::err("the file does not exists");
        network::sendResponse(*socket, ResponseId::IOFileNotExists);
        return false;
    }
    if (!sfs.isRegularFile(complete_path)) {
        console::out::err("the path isn't a file");
        network::sendResponse(*socket, ResponseId::IONotFile);
        return false;
    }
    network::sendResponse(*socket, ResponseId::Ok);
    return true;
}

bool IOGetFileContentRH::openFile() {
    input_file = new std::ifstream{complete_path, std::ios::binary | std::ios::beg};
    if (input_file && *input_file && input_file->is_open()) {
        return true;
    }
    response = ResponseId::IOFileNotReadable;
    return false;
}

void IOGetFileContentRH::startTransmition() {
    console::out::inf("sending buffers");
    for (; curr_buffer_idx < buffer_count; ++curr_buffer_idx) {
        if (!sendNextBuffer()) {
            return;
        }
    }
    console::out::inf("successfully sent buffers");
}

bool IOGetFileContentRH::sendNextBuffer() const {
    std::vector<char> buffer(buffer_size);
    input_file->read(buffer.data(), buffer_size);
    const size_t bytes_read{static_cast<size_t>(input_file->gcount())};
    if (bytes_read == 0) {
        console::out::err("error while reading file");
        return false;
    }
    if (bytes_read != buffer_size) {
        buffer.resize(bytes_read);
    }
    network::sendBuffer(*socket, buffer, true);
    return checkFlag();
}

bool IOGetFileContentRH::checkFlag() const {
    uint8_t flag{};
    if (!network::readInt<uint8_t>(*socket, flag)) {
        console::out::err("error while reading flag");
        return false;
    }
    if (flag == NEXT_BUFFER_FLAG) {
        return true;
    } else if (flag == STOP_FLAG) {
        console::out::inf("client sent STOP_FLAG");
        return false;
    }
    console::out::err("unknown flag: " + std::to_string(flag));
    return false;
}

void IOGetFileContentRH::closeFile() {
    input_file->close();
    delete input_file;
}
