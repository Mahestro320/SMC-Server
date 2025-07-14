#include "io/console.hpp"
#include "io/directories.hpp"
#include "io/sfs.hpp"
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
        return dirs::S_NETWORK / path;
    } else if (user.role == Role::Admin) {
        return dirs::STRG / path;
    }
#pragma warning(pop)
    return path;
}

fs::path SFS::relativeToUserScope(const fs::path& path) const {
    const User& user{client.getUser()};
#pragma warning(push)
#pragma warning(disable : 26813)
    if (user.role == Role::User) {
        return relative(path, dirs::S_NETWORK);
    } else if (user.role == Role::Admin) {
#pragma warning(pop)
        return relative(path, dirs::STRG);
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
        return isInDirectory(path, dirs::STRG);
    } else if (user.role == Role::User) {
#pragma warning(pop)
        return isInDirectory(path, dirs::S_NETWORK) && !isOtherUserDirectory(path);
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

bool SFS::isOtherUserDirectory(const fs::path& path) const {
    if (!isInDirectory(path, dirs::S_NETWORK)) {
        return false;
    }
    const fs::path filename{path.filename()};
    const std::string filename_str{filename.string()};
    if (!filename_str.starts_with(std::string{user_fs_config::PERSO_DIR_PREFIX} + ".")) {
        return false;
    }
    const User& user{client.getUser()};
    return !filename_str.ends_with(user.name);
}
