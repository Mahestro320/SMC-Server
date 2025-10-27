#include "util/thread.hpp"

#include <sstream>
#include <thread>

std::string util::thread::getCurrentThreadIdAsString() {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}
