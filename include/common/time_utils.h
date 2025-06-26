#pragma once

#include <chrono>
#include <cstdint>

namespace time_utils {

inline std::uint64_t ticks_ns() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

inline std::uint64_t frequency_ns() { return 1000000000ULL; }

inline std::uint64_t milliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

} // namespace time_utils

