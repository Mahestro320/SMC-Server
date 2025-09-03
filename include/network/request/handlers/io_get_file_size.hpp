#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"
#include "network/response/id.hpp"

class IOGetFileSizeRH final : public RH {
  private:
    std::filesystem::path path{};
    uint64_t size{};
    ResponseId path_response{ResponseId::Ok};

    bool readTargetPath(boost::asio::ip::tcp::socket& socket);
    void getFileSize();
    bool sendPathResponse(boost::asio::ip::tcp::socket& socket) const;
    bool sendFileSize(boost::asio::ip::tcp::socket& socket) const;

  public:
    IOGetFileSizeRH() = default;

    void run() override;
};
