#include "util/error_message.hpp"

std::string util::error_message::get(int errnum) {
    char buffer[0xFF];
    if (strerror_s(buffer, sizeof(buffer), errnum) == 0) {
        return std::string{buffer};
    }
    return "Unknown error";
}
