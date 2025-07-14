#include <fstream>
#include <iostream>
#include <termcolor.hpp>
#include "io/console.hpp"
#include "io/directories.hpp"
#include "io/logger.hpp"
#include "util/time.hpp"

namespace fs = std::filesystem;

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

void Logger::init() {
    if (!fs::exists(dirs::LOGS)) {
        fs::create_directories(dirs::LOGS);
    }
    setLogPath();
}

void Logger::setLogPath() {
    log_path = dirs::LOGS / (std::to_string(util::time::getMillis()) + "." + std::string(LOG_EXT.data()));
}

void Logger::log(const std::string& data) const {
    std::ofstream file{log_path, std::ios::app};
    if (!file) {
        std::cerr << termcolor::red << "error: logger: can't write in log file " << log_path << termcolor::reset
                  << std::endl;
        return;
    }
    file << data;
    file.close();
}
