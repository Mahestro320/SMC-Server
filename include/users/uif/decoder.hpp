#pragma once

#include <filesystem>
#include <vector>
#include "values.hpp"

// UIF = User Informations

class UIFDecoder final {
  private:
    static constexpr char SIGNATURE[4]{'U', 'I', 'F', 0x00};

    static constexpr uint64_t DATA_SIZE{0x210};
    static constexpr uint64_t SIGNATURE_OFFSET{0x00};
    static constexpr uint64_t ROLE_OFFSET{0x04};
    static constexpr uint64_t USERNAME_BLOCK_OFFSET{0x10};
    static constexpr uint64_t USERNAME_BLOCK_SIZE{0xFF};
    static constexpr uint64_t PASSWORD_BLOCK_OFFSET{0x110};
    static constexpr uint64_t PASSWORD_BLOCK_SIZE{0xFF};

    UIFValues values{};
    std::vector<char> data{};

    bool checkSize() const;
    bool checkSignature() const;
    void getRole();
    void getUsername();
    void getPassword();

  public:
    UIFDecoder() = default;

    bool loadDataFromPath(const std::filesystem::path& path);
    bool decode();
    const UIFValues& getValues() const;
};
