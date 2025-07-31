#pragma once

#define _WIN32_WINNT 0x0601

#include <boost/asio.hpp>
#include "../handler.hpp"

class LogoutRH : public RH {
  private:
    bool sendBeginResponse(boost::asio::ip::tcp::socket& socket);

  public:
    LogoutRH() = default;

    void run() override;
};
