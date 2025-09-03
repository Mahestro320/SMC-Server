#include "io/sfs.hpp"

#include "io/console.hpp"
#include "io/directories.hpp"
#include "network/client.hpp"
#include "users/fs_config.hpp"

namespace fs = std::filesystem;

SFS::SFS(Client& client) : client{client} {
}

fs::path SFS::absoluteFromUserScope(const fs::path& path) const {
    const User& user{client.getUser()};
#pragma warning(push)
#pragma warning(disable : 26813)
    if (user.role == Role::User) {
        return dirs::s_network / path;
    } else if (user.role == Role::Admin) {
        return dirs::strg / path;
    }
#pragma warning(pop)
    return path;
}

fs::path SFS::relativeToUserScope(const fs::path& path) const {
    const User& user{client.getUser()};
#pragma warning(push)
#pragma warning(disable : 26813)
    if (user.role == Role::User) {
        return relative(path, dirs::s_network);
    } else if (user.role == Role::Admin) {
#pragma warning(pop)
        return relative(path, dirs::strg);
    }
    return path;
}

fs::path SFS::relative(const fs::path& path, const fs::path& base) const {
    std::error_code ec{};
    const fs::path rel_path{fs::relative(path, base, ec)};
    if (ec) {
        console::out::err(ec.message());
        return path;
    }
    return rel_path;
}

fs::path SFS::real(const fs::path& path) const {
    std::error_code ec{};
    const fs::path real_path{fs::weakly_canonical(path, ec)};
    if (ec) {
        console::out::err(ec.message());
        return path;
    }
    return real_path;
}

fs::path SFS::removeEndBackslash(const fs::path& path) const {
    std::string path_str{path.string()};
    if (!path_str.empty() && (path_str.back() == '\\' || path_str.back() == '/')) {
        path_str.pop_back();
    }
    return fs::path{path_str};
}

std::vector<fs::path> SFS::getDirectoryFileList(const fs::path& path) const {
    if (!isDirectory(path)) {
        console::out::err("not a directory: " + path.string());
        return {};
    }
    std::error_code ec{};
    std::vector<fs::path> result{};
    for (const fs::path& file : fs::directory_iterator{path, ec}) {
        if (canLexicallyAccess(file)) {
            result.push_back(file);
        }
    }
    if (ec) {
        console::out::err(ec.message());
    }
    return result;
}

FileType SFS::getFileType(const fs::path& path) const {
    if (isRegularFile(path)) {
        return FileType::File;
    } else if (isDirectory(path)) {
        return FileType::Directory;
    }
    return FileType::None;
}

uint64_t SFS::getFileSize(const fs::path& path) const {
    std::error_code ec{};
    uint64_t size{fs::file_size(path, ec)};
    if (ec) {
        console::out::err(ec.message());
        return 0U;
    }
    return size;
}

bool SFS::isInDirectory(const fs::path& path, const fs::path& directory) const {
    const fs::path abs_path{real(path)};
    const fs::path abs_dir{real(directory)};

    auto path_it{abs_path.begin()};
    for (auto dir_it{abs_dir.begin()}; dir_it != abs_dir.end(); ++dir_it, ++path_it) {
        if (path_it == abs_path.end() || *path_it != *dir_it) {
            return false;
        }
    }
    return true;
}

bool SFS::canLexicallyAccess(const fs::path& path) const {
    if (!client.isLogged()) {
        return false;
    }
    const User& user{client.getUser()};
#pragma warning(push)
#pragma warning(disable : 26813)
    if (user.role == Role::Developer) {
        return true;
    } else if (user.role == Role::Admin) {
        return isInDirectory(path, dirs::strg) && !isUIFFile(path);
    } else if (user.role == Role::User) {
#pragma warning(pop)
        return isSame(path, dirs::s_network) || (isInUPCDirectory(path) || isInDirectory(path, dirs::s_network_shared));
    }
    return true;
}

bool SFS::exists(const fs::path& path) const {
    std::error_code ec{};
    const bool does_exists{fs::exists(path, ec) && canLexicallyAccess(path)};
    if (ec) {
        console::out::err(ec.message());
    }
    return !ec && does_exists;
}

bool SFS::isSame(const fs::path& first, const fs::path& second) const {
    const fs::path first_normal{removeEndBackslash(first.lexically_normal())};
    const fs::path second_normal{removeEndBackslash(second.lexically_normal())};
    console::out::inf(
        "first: " + first_normal.generic_string() + ", second: " + second_normal.generic_string() +
        ", is same: " + ((first_normal.generic_string() == second_normal.generic_string()) ? "true" : "false"));
    return first_normal.generic_string() == second_normal.generic_string();
}

bool SFS::isRegularFile(const fs::path& path) const {
    std::error_code ec{};
    const bool is_regular_file{exists(path) && fs::is_regular_file(path, ec)};
    if (ec) {
        console::out::err(ec.message());
    }
    return !ec && is_regular_file;
}

bool SFS::isDirectory(const fs::path& path) const {
    std::error_code ec{};
    const bool is_regular_file{exists(path) && fs::is_directory(path, ec)};
    if (ec) {
        console::out::err(ec.message());
    }
    return !ec && is_regular_file;
}

bool SFS::isInUPCDirectory(const fs::path& path) const {
    const User& user{client.getUser()};
    return isInDirectory(path, dirs::s_network / fs::path{"perso." + user.name});
}

bool SFS::isPUDDirectory(const fs::path& path) const {
    return isInDirectory(path, dirs::s_users) && path != dirs::s_users && isDirectory(path);
}

bool SFS::isUIFFile(const fs::path& path) const {
    return isPUDDirectory(path.parent_path()) && path.filename() == ".uif";
}
