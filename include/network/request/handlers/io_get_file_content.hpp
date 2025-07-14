#pragma once

#include <fstream>
#include "../handler.hpp"
#include "network/response/id.hpp"

class IOGetFileContentRH final : public RH {
  private:
    static inline constexpr uint8_t NEXT_BUFFER_FLAG{0xAB};
    static inline constexpr uint64_t MAX_BUFFER_SIZE{0xFFFF};

    boost::asio::ip::tcp::socket* socket{};
    uint64_t buffer_size{}, buffer_count{};
    uint64_t file_size{};
    std::filesystem::path base_path{}, complete_path{};
    std::ifstream* input_file;

    uint64_t curr_buffer_idx{};
    ResponseId response{ResponseId::Ok};

    bool readBufferSize();
    bool checkBufferSize();
    bool readPath();
    bool checkPath();
    void getFileSize();
    void calcBufferCount();
    bool openFile();
    void startTransmition();
    bool sendNextBuffer() const;
    void closeFile();

  public:
    IOGetFileContentRH() = default;

    void run() override;
};
