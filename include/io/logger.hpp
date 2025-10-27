#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include <fstream>
#include <string>

class Logger final {
    static constexpr std::string_view LOG_EXT{"log"};
    std::filesystem::path log_path{};
    std::ofstream* file{};

    void setLogPath();

    ~Logger();
    Logger() = default;

  public:
    static Logger& getInstance();

    void init();
    std::ofstream& getStream();
};
