#pragma once

#include <source_location>
#include <string>
#include "common.hpp"

namespace console::out {

#ifdef DEBUG
extern void ptr(const void* ptr, const std::source_location& location = std::source_location::current());
extern void debug(const std::string& inf = "", const std::source_location& location = std::source_location::current());
#endif
extern void inf(const std::string& inf = "", const std::source_location& location = std::source_location::current());
extern void warn(const std::string& warn = "", const std::source_location& location = std::source_location::current());
extern void err(const std::string& err = "", const std::source_location& location = std::source_location::current());
extern void err(const std::exception& e, const std::source_location& location = std::source_location::current());

} // namespace console::out
