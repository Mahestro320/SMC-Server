#pragma once

#include <filesystem>
#include <string>

class Logger final {
    static constexpr std::string_view LOG_EXT{"log"};
    std::filesystem::path log_path{};

    void setLogPath();

    Logger() = default;

  public:
    static Logger& getInstance();

    void init();
    void log(const std::string& data) const;
};
