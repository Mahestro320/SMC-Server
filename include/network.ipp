#pragma once

#include "io/console.hpp"

template<std::integral T> bool network::sendInt(boost::asio::ip::tcp::socket& socket, T byte) {
    boost::system::error_code error_code{};
    boost::asio::write(socket, boost::asio::buffer(&byte, sizeof(T)), error_code);
    if (error_code) {
        console::out::err("error while sending int: " + error_code.message());
        return false;
    }
    return true;
}

template<std::integral T> bool network::readInt(boost::asio::ip::tcp::socket& socket, T& byte) {
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