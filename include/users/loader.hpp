#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include "user.hpp"
#include "users/uif/values.hpp"

class UserLoader final {
  private:
    static constexpr char UIF_FILE_NAME[5]{".uif"};

    const std::filesystem::path& folder_path{};
    User user{};

    void loadUserFromUIFValues(const UIFValues& values);
    bool createPersoDirectory() const;

  public:
    UserLoader(const std::filesystem::path& folder_path);

    bool load();
    User getUser() const;
};
