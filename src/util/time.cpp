#include "util/time.hpp"

#include <chrono>
#include <format>
#include "server/config.hpp"

namespace {

template<typename duration, std::enable_if_t<std::chrono::_Is_duration_v<duration>, bool> = true>
uint64_t getTimestamp() {
    const auto now{std::chrono::system_clock::now()};
    const auto time{std::chrono::duration_cast<duration>(now.time_since_epoch())};
    return time.count();
}

} // namespace

uint64_t util::time::getSeconds() {
    return getTimestamp<std::chrono::seconds>();
}

uint64_t util::time::getMillis() {
    return getTimestamp<std::chrono::milliseconds>();
}

uint64_t util::time::getNanos() {
    return getTimestamp<std::chrono::nanoseconds>();
}

std::string util::time::formatTime(uint64_t timestamp, const std::string& format) {
    const std::chrono::sys_seconds tp{std::chrono::seconds{timestamp}};
    if (!format.empty()) {
        return std::vformat(format, std::make_format_args(tp));
    }
    Config& config{Config::getInstance()};
    const ConfigValues& values{config.getValues()};
    return std::vformat(values.time_format, std::make_format_args(tp));
}
