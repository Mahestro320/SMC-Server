#pragma once

#include "system/beg.hpp"

#include <stdint.h>

namespace dwl_upl::constants {

static inline constexpr size_t MAX_BUFFER_SIZE{0xFFFF};

namespace tr_flags { // transmition flags

static inline constexpr uint8_t NEXT_BUFFER{0x00};
static inline constexpr uint8_t STOP{0xFF};

} // namespace tr_flags

} // namespace dwl_upl::constants
