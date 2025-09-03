#include "io/console.hpp"
#include "network.hpp"

using boost::asio::ip::tcp;

namespace {

void pinf(const std::string& msg, bool silent) {
    if (!silent) {
        console::out::inf(msg);
    }
}

} // namespace

bool network::sendResponse(tcp::socket& socket, ResponseId id, bool silent) {
    const uint8_t byte{static_cast<uint8_t>(id)};
    pinf("sending response: <:color=bright_blue>" + std::to_string(byte) + "<:color=reset> (<:color=green>" +
             response::getName(id) + "<:color=reset>)",
         silent);
    return sendInt(socket, byte);
}

bool network::sendBuffer(tcp::socket& socket, const std::vector<char>& buffer, bool silent) {
    const uint64_t buffer_size{buffer.size()};
    pinf("sending buffer size (<:color=bright_blue>" + std::to_string(buffer_size) + "<:color=reset>)", silent);
    if (!sendInt(socket, buffer_size)) {
        return false;
    }
    pinf("sending buffer content", silent);
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

bool network::sendString(tcp::socket& socket, const std::string& str, bool silent) {
    pinf("sending string", silent);
    return sendBuffer(socket, std::vector<char>(str.begin(), str.end()));
}

bool network::readBuffer(tcp::socket& socket, std::vector<char>& buffer, bool silent) {
    pinf("reading buffer size", silent);
    boost::system::error_code error_code{};
    size_t buffer_size{};
#if SIZE_MAX == UINT64_MAX
    if (!readInt<uint64_t>(socket, buffer_size)) {
        return false;
    }
#else
    uint64_t buffer_size_64{};
    if (!readInt<uint64_t>(socket, buffer_size_64)) {
        return false;
    }
    buffer_size = static_cast<size_t>(buffer_size_64);
#endif
    buffer.resize(buffer_size);
    pinf("reading buffer content", silent);
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

bool network::readString(boost::asio::ip::tcp::socket& socket, std::string& str, bool silent) {
    pinf("reading string", silent);
    std::vector<char> data(str.begin(), str.end());
    if (!readBuffer(socket, data)) {
        return false;
    }
    str = std::string(data.begin(), data.end());
    pinf("string content: <:color=green>\"" + str + "\"<:color=reset>", silent);
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
