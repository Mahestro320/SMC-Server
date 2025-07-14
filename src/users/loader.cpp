#include "io/console.hpp"
#include "io/directories.hpp"
#include "users/fs_config.hpp"
#include "users/loader.hpp"
#include "users/role.hpp"
#include "users/uif/decoder.hpp"

namespace fs = std::filesystem;

UserLoader::UserLoader(const fs::path& folder_path) : folder_path{folder_path} {
}

bool UserLoader::load() {
    const fs::path rel_folder_path{fs::relative(folder_path, dirs::STRG)};
    UIFDecoder uif_decoder{};
    if (!uif_decoder.loadDataFromPath(folder_path / UIF_FILE_NAME) || !uif_decoder.decode()) {
        console::out::err("error while loading user \"" + rel_folder_path.string() + "\"");
        return false;
    }
    loadUserFromUIFValues(uif_decoder.getValues());
    user.perso_directory = dirs::S_NETWORK / (std::string{user_fs_config::PERSO_DIR_PREFIX} + "." + user.name);
    if (!createPersoDirectory()) {
        return false;
    }
    console::out::inf("loaded user " + user.name + " (" + role::getName(user.role) + ")");
    return true;
}

void UserLoader::loadUserFromUIFValues(const UIFValues& values) {
    user.name = values.username;
    user.password = values.password;
    user.role = static_cast<Role>(values.role);
}

bool UserLoader::createPersoDirectory() const {
    if (!fs::exists(user.perso_directory) && !fs::create_directories(user.perso_directory)) {
        console::out::err("error while creating perso dir for user " + user.name);
        return false;
    }
    return true;
}

User UserLoader::getUser() const {
    return user;
}
