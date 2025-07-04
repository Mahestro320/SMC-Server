#pragma once

#include <filesystem>
#include "file_type.hpp"

class Client;

// Secured Filesystem
class SFS final {
  private:
    Client& client;

  public:
    SFS(Client& client);

    std::filesystem::path absoluteFromUserScope(const std::filesystem::path& path) const;
    std::filesystem::path relativeToUserScope(const std::filesystem::path& path) const;
    std::filesystem::path relative(const std::filesystem::path& path, const std::filesystem::path& base) const;
    std::filesystem::path real(const std::filesystem::path& path) const;
    std::vector<std::filesystem::path> getDirectoryFileList(const std::filesystem::path& path) const;
    FileType getFileType(const std::filesystem::path& path) const;

    bool isInDirectory(const std::filesystem::path& path, const std::filesystem::path& directory) const;
    bool canAccess(const std::filesystem::path& path) const;
    bool exists(const std::filesystem::path& path) const;
    bool isRegularFile(const std::filesystem::path& path) const;
    bool isDirectory(const std::filesystem::path& path) const;
    bool isOtherUserDirectory(const std::filesystem::path& path) const;
};
