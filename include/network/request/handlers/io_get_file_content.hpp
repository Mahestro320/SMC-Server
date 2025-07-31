#pragma once

#include <boost/asio.hpp>
#include <fstream>
#include "../handler.hpp"
#include "network/response/id.hpp"

class IOGetFileContentRH final : public RH {
  private:
    static inline constexpr uint8_t NEXT_BUFFER_FLAG{0x00};
    static inline constexpr uint8_t STOP_FLAG{0xFF};
    static inline constexpr uint64_t MAX_BUFFER_SIZE{0xFFFF};

    boost::asio::ip::tcp::socket* socket{};
    uint64_t buffer_size{}, buffer_count{};
    uint64_t file_size{};
    std::filesystem::path base_path{}, complete_path{};
    std::ifstream* input_file;

    uint64_t curr_buffer_idx{};
    ResponseId response{ResponseId::Ok};

    bool init();
    void getFileSize();
    void calcBufferCount();
    bool tryToOpenFile();
    bool readBufferSize();
    bool checkBufferSize();
    bool readPath();
    bool checkPath();
    bool openFile();
    void startTransmition();
    bool sendNextBuffer() const;
    bool checkFlag() const;
    void closeFile();

  public:
    IOGetFileContentRH() = default;

    void run() override;
};
