#include "util/version.hpp"

std::string Version::toString(char delim) const {
    return std::to_string(MAJOR) + std::string{delim} + std::to_string(MINOR) + std::string{delim} +
           std::to_string(PATCH);
}
