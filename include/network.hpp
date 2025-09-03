#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <concepts>
#include "network/response/id.hpp"

namespace network {

extern bool sendResponse(boost::asio::ip::tcp::socket& socket, ResponseId id, bool silent = false);
extern bool sendBuffer(boost::asio::ip::tcp::socket& socket, const std::vector<char>& buffer, bool silent = false);
extern bool sendString(boost::asio::ip::tcp::socket& socket, const std::string& str, bool silent = false);
extern bool readBuffer(boost::asio::ip::tcp::socket& socket, std::vector<char>& buffer, bool silent = false);
extern bool readString(boost::asio::ip::tcp::socket& socket, std::string& str, bool silent = false);

template<std::integral T> bool sendInt(boost::asio::ip::tcp::socket& socket, T byte);
template<std::integral T> bool readInt(boost::asio::ip::tcp::socket& socket, T& byte);

extern std::string getIpv4Address();

} // namespace network

#include "network.ipp"
