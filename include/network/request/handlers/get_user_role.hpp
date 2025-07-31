#pragma once

#include <boost/asio.hpp>
#include "../handler.hpp"

class GetUserRoleRH final : public RH {
  private:
    void sendUserRole(boost::asio::ip::tcp::socket& socket);

  public:
    GetUserRoleRH() = default;

    void run() override;
};
