#include <sstream>
#include <thread>
#include "util/thread.hpp"

std::string util::thread::getCurrentThreadIdAsString() {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}
