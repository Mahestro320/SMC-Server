#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include <fstream>
#include "../handler.hpp"
#include "network/response/id.hpp"

class IOSetFileContentRH : public RH {
  private:
    boost::asio::ip::tcp::socket* socket{};
    std::filesystem::path raw_path{}, complete_path{};
    size_t buffer_size{}, buffer_count{};
    uint64_t file_size{};
    std::vector<char> curr_buffer{};
    uint64_t curr_buffer_idx{};
    std::ofstream* output_file{};
    ResponseId response{};

    bool init();
    bool readFileSize();
    bool readBufferSize();
    bool checkBufferSize() const;
    bool readPath();
    bool checkPath() const;
    void calcBufferCount();
    bool tryToOpenFile();
    bool openFile();
    void startTransmition();
    bool readNextBuffer();
    bool writeCurrBufferIntoFile();
    void resizeBufferIfNeeded();
    bool checkFlag() const;
    void closeFile();

  public:
    IOSetFileContentRH() = default;
    ~IOSetFileContentRH();

    void run() override;
};
