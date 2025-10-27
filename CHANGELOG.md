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
- modified logger and console

## Version 0.3.0

- removing bug of architecture check by always importing `system/beg.hpp`
- adding sysvars and properties
- renaming class `EnvVarStrResolver` to `EVSStrResolver`
- adding text colors
- adding request handler `run_syscmd`
- removing bug if client isn't logged for handlers
- the logger opens now the file at initialization
- removing SFS security bug
- adding request handler `io_create_file`
- adding request handler `io_create_directory`
- adding request handler `io_set_file_content`

## Version 0.4.0

- adding request handler `io_delete_file`
- adding request handler `get_version`
- modifying request ids: the first is now `GetVersion` for compatibility
