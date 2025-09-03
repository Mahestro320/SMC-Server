#pragma once

#include "system/beg.hpp"

#include <string>

struct Version {
    uint16_t MAJOR{}, MINOR{}, PATCH{};

    std::string toString(char delim = '.') const;
};
