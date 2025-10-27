#include "network/request/handlers/get_version.hpp"

#include "common.hpp"
#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

void GetVersionRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendResponse(socket, ResponseId::Ok)) {
        return;
    }
    network::sendString(socket, common::VERSION.toString());
}
