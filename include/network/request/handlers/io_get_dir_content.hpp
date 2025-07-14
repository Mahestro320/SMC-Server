#pragma once

#include "../handler.hpp"
#include "io/file_info.hpp"
#include "network/response/id.hpp"

class IOGetDirContentRH : public RH {
  private:
    std::vector<FileInfo> data{};
    std::filesystem::path path{};

    bool getPath(boost::asio::ip::tcp::socket& socket);
    void getFileList();
    void pushFileInList(const std::filesystem::path& path);
    bool sendBuffer(boost::asio::ip::tcp::socket& socket) const;
    ResponseId checkPath() const;

  public:
    IOGetDirContentRH() = default;

    void run() override;
};
