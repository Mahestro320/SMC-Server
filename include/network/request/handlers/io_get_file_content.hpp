#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include <fstream>
#include "../handler.hpp"
#include "network/response/id.hpp"

class IOGetFileContentRH final : public RH {
  private:
    boost::asio::ip::tcp::socket* socket{};
    uint64_t buffer_size{}, buffer_count{};
    uint64_t file_size{};
    std::filesystem::path raw_path{}, complete_path{};
    std::ifstream* input_file;

    std::vector<char> curr_buffer{};
    uint64_t curr_buffer_idx{};
    ResponseId response{ResponseId::Ok};

    bool init();
    void getFileSize();
    void calcBufferCount();
    bool tryToOpenFile();
    bool sendFileSize() const;
    bool readBufferSize();
    bool checkBufferSize();
    bool readPath();
    bool checkPath();
    bool openFile();
    void startTransmition();
    bool readNextBufferFromFile();
    bool sendCurrBuffer() const;
    bool checkFlag() const;
    void closeFile();

  public:
    IOGetFileContentRH() = default;
    ~IOGetFileContentRH();

    void run() override;
};
