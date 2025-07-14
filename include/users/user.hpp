#pragma once

#include <filesystem>
#include "role.hpp"

struct User final {
    std::string name{}, password{};
    Role role{Role::None};
    uint8_t wrong_password_count{};
    std::filesystem::path perso_directory{};

    bool is_valid{true};
};
