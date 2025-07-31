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
- use `acceptor.async_accept` instead of `acceptor.accept` in `main.cpp` and refractoring code
- removing variable `inline bool common::is_running`
- removing unused code in `console.hpp` and `console.cpp`
- renaming 5 variables in `directories.hpp`
- adding configuration file
- replacing flag value `0xAB` to `0x00` for variable `IOGetFileContentRH::NEXT_BUFFER_FLAG`
- adding flag `IOGetFileContentRH::STOP_FLAG`
- refactoring `get_user_role` request handler
- refactoring `io_file_exists` request handler
- refactoring `io_get_dir_content` request handler
- refactoring `io_get_file_content` request handler
- refactoring `io_get_file_size` request handler
- refactoring `io_get_file_type` request handler
- refactoring `io_get_real_path` request handler
- refactoring `login` request handler
- refactoring `logout` request handler
- refactoring `network/request/handler`
- adding security check for `io/sfs`

## Version 0.2.2

- adding architecture check
- x86-64 compatibility
