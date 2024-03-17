#pragma once

#include <bit>
#include <concepts>

namespace yLAB {

  template <std::unsigned_integral T>
  constexpr inline int log2_floor(T number) noexcept {
    return std::bit_width(number) - 1;
  }

} // <--- namespace yLAB

