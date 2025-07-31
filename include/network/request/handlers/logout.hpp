#pragma once

#include <boost/asio.hpp>
#include "../handler.hpp"

class LogoutRH : public RH {
  private:
    bool sendBeginResponse(boost::asio::ip::tcp::socket& socket);

  public:
    LogoutRH() = default;

    void run() override;
};
