#pragma once

#define _WIN32_WINNT 0x0601

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"

class IOGetFileTypeRH final : public RH {
  private:
    std::filesystem::path path{};

    bool getPath(boost::asio::ip::tcp::socket& socket);
    bool checkPath(boost::asio::ip::tcp::socket& socket) const;

  public:
    IOGetFileTypeRH() = default;

    void run() override;
};
