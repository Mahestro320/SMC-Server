#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"
#include "network/response/id.hpp"

class IOCreateFileRH final : public RH {
  private:
    std::filesystem::path path{};
    ResponseId response{};

    bool readPath(boost::asio::ip::tcp::socket& socket);
    bool checkPath();
    bool createFile();

  public:
    IOCreateFileRH() = default;

    void run() override;
};
