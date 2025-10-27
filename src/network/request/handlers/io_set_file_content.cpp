#include "network/request/handlers/io_set_file_content.hpp"

#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/util/dwl_upl.hpp"

namespace fs = std::filesystem;

IOSetFileContentRH::~IOSetFileContentRH() {
    if (output_file) {
        output_file->close();
    }
    delete output_file;
}

void IOSetFileContentRH::run() {
    socket = &client->getSocket();
    if (!init()) {
        return;
    }
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

bool IOSetFileContentRH::init() {
    if (!client->isLogged()) {
        console::out::err("the client isn't logged");
        network::sendResponse(*socket, ResponseId::NotLogged);
        return false;
    }
    return network::sendResponse(*socket, ResponseId::Ok) && readFileSize() && readBufferSize() && checkBufferSize() &&
           readPath() && checkPath();
}

bool IOSetFileContentRH::readFileSize() {
    return network::readInt<uint64_t>(*socket, file_size);
}

bool IOSetFileContentRH::readBufferSize() {
    uint64_t buffer_size_64{};
    if (!network::readInt<uint64_t>(*socket, buffer_size_64)) {
        return false;
    }
    buffer_size = static_cast<size_t>(buffer_size_64);
    return true;
}

bool IOSetFileContentRH::checkBufferSize() const {
    if (buffer_size == 0 || buffer_size > dwl_upl::constants::MAX_BUFFER_SIZE) {
        network::sendResponse(*socket, ResponseId::InvalidBufferSize);
        return false;
    }
    network::sendResponse(*socket, ResponseId::Ok);
    return true;
}

bool IOSetFileContentRH::readPath() {
    std::string path_str{};
    if (!network::readString(*socket, path_str)) {
        return false;
    }
    raw_path = fs::path{path_str};
    const SFS& sfs{client->getSFS()};
    complete_path = sfs.absoluteFromUserScope(raw_path);
    return true;
}

bool IOSetFileContentRH::checkPath() const {
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

void IOSetFileContentRH::calcBufferCount() {
    buffer_count = static_cast<size_t>(std::ceil(static_cast<double>(file_size) / buffer_size));
    console::out::inf("buffer count: " + std::to_string(buffer_count));
}

bool IOSetFileContentRH::tryToOpenFile() {
    bool success{};
    try {
        success = openFile();
        network::sendResponse(*socket, response);
    } catch (const std::exception& e) {
        network::sendResponse(*socket, ResponseId::IOFileNotReadable);
        console::out::err(e);
        return false;
    }
    return success;
}

bool IOSetFileContentRH::openFile() {
    output_file = new std::ofstream{complete_path, std::ios::binary | std::ios::beg};
    if (output_file && *output_file && output_file->is_open()) {
        return true;
    }
    response = ResponseId::IOFileNotReadable;
    return false;
}

void IOSetFileContentRH::startTransmition() {
    console::out::inf("reading buffers");
    for (; curr_buffer_idx < buffer_count; ++curr_buffer_idx) {
        if (!readNextBuffer() || !writeCurrBufferIntoFile()) {
            return;
        }
    }
    console::out::inf("successfully readed buffers");
}

bool IOSetFileContentRH::readNextBuffer() {
    if (curr_buffer.size() != buffer_size) {
        curr_buffer.resize(buffer_size);
    }
    boost::system::error_code ec{};
    boost::asio::read(*socket, boost::asio::buffer(curr_buffer, buffer_size), ec);
    if (ec) {
        console::out::err("error while reading buffer " + std::to_string(curr_buffer_idx) + ": " + ec.message());
        return false;
    }
    return checkFlag();
}

bool IOSetFileContentRH::writeCurrBufferIntoFile() {
    resizeBufferIfNeeded();
    output_file->write(curr_buffer.data(), curr_buffer.size());
    if (!*output_file) {
        console::out::err("error while writing into file");
        return false;
    }
    curr_buffer = std::vector<char>(buffer_size);
    return true;
}

void IOSetFileContentRH::resizeBufferIfNeeded() {
    if (curr_buffer_idx + 1 == buffer_count) {
        curr_buffer.resize(static_cast<size_t>(file_size) - (buffer_count - 1) * buffer_size);
    }
}

bool IOSetFileContentRH::checkFlag() const {
    uint8_t flag{};
    if (!network::readInt<uint8_t>(*socket, flag)) {
        console::out::err("error while reading flag");
        return false;
    }
    if (flag == dwl_upl::constants::tr_flags::NEXT_BUFFER) {
        return true;
    } else if (flag == dwl_upl::constants::tr_flags::STOP) {
        console::out::inf("client sent STOP_FLAG");
        return false;
    }
    console::out::err("client sent an unknown flag: " + std::to_string(flag));
    return false;
}

void IOSetFileContentRH::closeFile() {
    output_file->close();
    delete output_file;
}
