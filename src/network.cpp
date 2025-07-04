#include "io/console.hpp"
#include "network.hpp"

using boost::asio::ip::tcp;

bool network::sendResponse(tcp::socket& socket, ResponseId id) {
    const uint8_t byte{static_cast<uint8_t>(id)};
    console::out::inf("sending response: " + std::to_string(byte) + " (" + response::getName(id) + ")");
    return sendInt(socket, byte);
}

bool network::sendBuffer(tcp::socket& socket, const std::vector<char>& buffer) {
    const uint64_t buffer_size{buffer.size()};
    console::out::inf("sending buffer size (" + std::to_string(buffer_size) + ")");
    if (!sendInt(socket, buffer_size)) {
        return false;
    }
    console::out::inf("sending buffer content");
    boost::system::error_code error_code{};
    uint64_t bytes_write{boost::asio::write(socket, boost::asio::buffer(buffer), error_code)};
    if (error_code) {
        console::out::err("error while sending buffer content: " + error_code.message());
        return false;
    }
    if (bytes_write < buffer_size) {
        console::out::err("the buffer isn't fully sent !");
        return false;
    }
    return true;
}

bool network::sendString(tcp::socket& socket, const std::string& str) {
    console::out::inf("sending string");
    return sendBuffer(socket, std::vector<char>(str.begin(), str.end()));
}

bool network::readBuffer(tcp::socket& socket, std::vector<char>& buffer) {
    console::out::inf("reading buffer size");
    boost::system::error_code error_code{};
    uint64_t buffer_size{};
    if (!readInt(socket, buffer_size)) {
        return false;
    }
    buffer.resize(buffer_size);
    console::out::inf("reading buffer content");
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(buffer, buffer_size), error_code)};
    if (error_code) {
        console::out::err(error_code.message());
        return false;
    }
    if (bytes_read < buffer_size) {
        console::out::err("the buffer isn't fully readed !");
        return false;
    }
    return true;
}

bool network::readString(boost::asio::ip::tcp::socket& socket, std::string& str) {
    console::out::inf("reading string");
    std::vector<char> data(str.begin(), str.end());
    if (!readBuffer(socket, data)) {
        return false;
    }
    str = std::string(data.begin(), data.end());
    console::out::inf("string content: \"" + str + "\"");
    return true;
}

template<std::integral T> bool network::sendInt(tcp::socket& socket, T data) {
    boost::system::error_code error_code{};
    boost::asio::write(socket, boost::asio::buffer(&data, sizeof(T)), error_code);
    if (error_code) {
        console::out::err("error while sending int: " + error_code.message());
        return false;
    }
    return true;
}

template<std::integral T> bool network::readInt(tcp::socket& socket, T& data) {
    boost::system::error_code error_code{};
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(&data, sizeof(T)), error_code)};
    if (error_code) {
        console::out::err("error while reading int: " + error_code.message());
        return false;
    }
    if (bytes_read < sizeof(T)) {
        console::out::err("the int isn't fully readed !");
        return false;
    }
    return true;
}

std::string network::getIpv4Address() {
    boost::asio::io_context io_ctx{};
    std::string hostname{boost::asio::ip::host_name()};
    tcp::resolver resolver{io_ctx};
    boost::system::error_code error_code{};
    tcp::resolver::results_type results{resolver.resolve(hostname, "", error_code)};
    if (error_code) {
        return "unknown";
    }
    for (const auto& entry : results) {
        auto endpoint{entry.endpoint()};
        boost::asio::ip::address address{endpoint.address()};
        if (address.is_v4()) {
            return address.to_string();
        }
    }
    return "unknown";
}
