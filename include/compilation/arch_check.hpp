#pragma once

#if !defined(_WIN64) && !defined(_WIN32)
#pragma error("unknown architecture: please compile it under windows x86-64")
#endif
