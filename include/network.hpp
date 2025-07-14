#pragma once

#include <boost/asio.hpp>
#include <concepts>
#include "network/response/id.hpp"

namespace network {

extern bool sendResponse(boost::asio::ip::tcp::socket& socket, ResponseId id, bool silent = false);
extern bool sendBuffer(boost::asio::ip::tcp::socket& socket, const std::vector<char>& buffer, bool silent = false);
extern bool sendString(boost::asio::ip::tcp::socket& socket, const std::string& str, bool silent = false);
extern bool readBuffer(boost::asio::ip::tcp::socket& socket, std::vector<char>& buffer, bool silent = false);
extern bool readString(boost::asio::ip::tcp::socket& socket, std::string& str, bool silent = false);

template<std::integral T> inline bool sendInt(boost::asio::ip::tcp::socket& socket, T byte) {
    boost::system::error_code error_code{};
    boost::asio::write(socket, boost::asio::buffer(&byte, sizeof(T)), error_code);
    if (error_code) {
        console::out::err("error while sending int: " + error_code.message());
        return false;
    }
    return true;
}

template<std::integral T> inline bool readInt(boost::asio::ip::tcp::socket& socket, T& byte) {
    boost::system::error_code error_code{};
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(&byte, sizeof(T)), error_code)};
    if (error_code) {
        console::out::err("error while reading int: " + error_code.message());
        return false;
    }
    if (bytes_read < sizeof(T)) {
        console::out::err("the int isn't fully readed");
        return false;
    }
    return true;
}

extern std::string getIpv4Address();

} // namespace network
