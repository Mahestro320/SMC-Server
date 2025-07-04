#include "io/console.hpp"
#include "io/directories.hpp"
#include "users/loader.hpp"
#include "users/manager.hpp"

namespace fs = std::filesystem;

UsersManager& UsersManager::getInstance() {
    static UsersManager instance{};
    return instance;
}

void UsersManager::load() {
    console::out::inf("loading users");
    if (!checkUsersFolderExistence()) {
        return;
    }
    uint32_t users_count{};
    uint32_t users_loaded_count{};
    for (const fs::path& path : fs::directory_iterator{dirs::S_USERS}) {
        ++users_count;
        UserLoader loader{path};
        if (!loader.load()) {
            continue;
        }
        ++users_loaded_count;
        users.push_back(loader.getUser());
    }
    if (users_count == 0) {
        console::out::inf("no user found");
        return;
    }
    console::out::inf(std::to_string(users_loaded_count) + "/" + std::to_string(users_count) + " users loaded");
}

bool UsersManager::checkUsersFolderExistence() const {
    if (fs::exists(dirs::S_USERS)) {
        return true;
    }
    console::out::err("the users directory does not exists");
    fs::create_directories(dirs::S_USERS);
    return false;
}

bool UsersManager::exists(const std::string& username) {
    return get(username);
}

User* UsersManager::get(const std::string& username) {
    for (User& user : users) {
        if (user.name == username) {
            return &user;
        }
    }
    return nullptr;
}
