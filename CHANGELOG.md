# Changelog

## Version 0.2.0

- function `User* UsersManager::get(const std::string& username)` is now `User UsersManager::get(const std::string& username)`
- adding request handlers `io_get_file_size` and `io_get_file_content`
- adding function `uint64_t SFS::getFileSize(const std::filesystem::path& path) const`
- usernames and passwords can now no longer exceed 16 characters
- functions `template<std::integral T> bool network::readInt(boost::asio::ip::tcp::socket& socket, T& byte)` and `template<std::integral T> bool network::sendInt(boost::asio::ip::tcp::socket& socket, T& byte)` is now inline
- renaming `bool SFS::canAccess(const fs::path& path) const` into `bool SFS::canLexicallyAccess(const fs::path& path) const`

## Version 0.2.1

- the `io_get_file_content` request handler buffer size check now rejects the request if the buffer size is 0
