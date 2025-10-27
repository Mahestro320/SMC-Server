#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include "file_type.hpp"

class Client;

/*
 * Here is what the abbreviated names in this code mean:
 *
 * SFS: Secured Filesystem
 * PUD: Protected User Data
 * UPC: User Personnal Content
 *
 */

class SFS final {
  private:
    Client& client;

  public:
    SFS(Client& client);

    std::filesystem::path absoluteFromUserScope(const std::filesystem::path& path) const;
    std::filesystem::path relativeToUserScope(const std::filesystem::path& path) const;
    std::filesystem::path relative(const std::filesystem::path& path, const std::filesystem::path& base) const;
    std::filesystem::path real(const std::filesystem::path& path) const;
    std::filesystem::path removeEndBackslash(const std::filesystem::path& path) const;
    std::vector<std::filesystem::path> getDirectoryFileList(const std::filesystem::path& path) const;
    FileType getFileType(const std::filesystem::path& path) const;
    uint64_t getFileSize(const std::filesystem::path& path) const;

    bool isInDirectory(const std::filesystem::path& path, const std::filesystem::path& directory) const;
    bool canLexicallyAccess(const std::filesystem::path& path) const;
    bool exists(const std::filesystem::path& path) const;
    bool isSame(const std::filesystem::path& first, const std::filesystem::path& second) const;
    bool isRegularFile(const std::filesystem::path& path) const;
    bool isDirectory(const std::filesystem::path& path) const;
    bool isInUPCDirectory(const std::filesystem::path& path) const;
    bool isPUDDirectory(const std::filesystem::path& path) const;
    bool isUIFFile(const std::filesystem::path& path) const;
};
