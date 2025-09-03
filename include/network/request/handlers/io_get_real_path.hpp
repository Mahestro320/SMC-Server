#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"

class IOGetRealPathRH : public RH {
  private:
    std::filesystem::path input_path{}, real_path{};

    bool getInputPath(boost::asio::ip::tcp::socket& socket);
    bool checkInputPath(boost::asio::ip::tcp::socket& socket) const;
    bool setRealPath();
    bool sendRealPath(boost::asio::ip::tcp::socket& socket) const;

  public:
    IOGetRealPathRH() = default;

    void run() override;
};
