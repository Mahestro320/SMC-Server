#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <string>
#include "../handler.hpp"
#include "network/response/id.hpp"
#include "users/user.hpp"

class LoginRH final : public RH {
  private:
    std::string username{}, password{};
    User user{};

    bool getUsername(boost::asio::ip::tcp::socket& socket);
    bool getPassword(boost::asio::ip::tcp::socket& socket);

    bool getUserFromUsername();
    bool checkPassword();

    ResponseId logClientFrom(boost::asio::ip::tcp::socket& socket);

  public:
    LoginRH() = default;

    void run() override;
};
