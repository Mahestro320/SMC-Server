#pragma once

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
