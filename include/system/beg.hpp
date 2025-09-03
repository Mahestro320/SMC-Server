#pragma once

#if !defined(_WIN32) && !defined(_WIN64)
#error "please compile it under windows x86-64"
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#if defined(__has_include)
#if !__has_include("boost/system.hpp")
#error the lib 'boost' isn't in 'external' directory
#endif
#if !__has_include("termcolor.hpp")
#error the lib 'termcolor' isn't in 'external' directory
#endif
#endif
