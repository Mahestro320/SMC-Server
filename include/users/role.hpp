#pragma once

#include <string>

enum class Role : uint8_t {
    User,
    Admin,
    Developer,
    None = 0xFF,
};

namespace role {

extern std::string getName(Role role);

}
