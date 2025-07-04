#pragma once

#include <boost/asio.hpp>
#include <concepts>
#include "network/response/id.hpp"

namespace network {

extern bool sendResponse(boost::asio::ip::tcp::socket& socket, ResponseId id);
extern bool sendBuffer(boost::asio::ip::tcp::socket& socket, const std::vector<char>& buffer);
extern bool sendString(boost::asio::ip::tcp::socket& socket, const std::string& str);
extern bool readBuffer(boost::asio::ip::tcp::socket& socket, std::vector<char>& buffer);
extern bool readString(boost::asio::ip::tcp::socket& socket, std::string& str);

template<std::integral T> extern bool sendInt(boost::asio::ip::tcp::socket& socket, T byte);
template<std::integral T> extern bool readInt(boost::asio::ip::tcp::socket& socket, T& byte);

extern std::string getIpv4Address();

} // namespace network
