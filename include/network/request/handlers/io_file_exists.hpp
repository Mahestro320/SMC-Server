#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"

class IOFileExistsRH final : public RH {
  private:
    std::filesystem::path path{};

    bool getPath(boost::asio::ip::tcp::socket& socket);
    bool checkPath(boost::asio::ip::tcp::socket& socket) const;

  public:
    IOFileExistsRH() = default;

    void run() override;
};
