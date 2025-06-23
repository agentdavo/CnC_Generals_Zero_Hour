#pragma once
#include <cstdint>
#if __has_include(<bit>)
#include <bit>
#endif

namespace common {

constexpr std::uint16_t byteswap16(std::uint16_t value) noexcept {
#if defined(__cpp_lib_byteswap)
    return std::byteswap(value);
#else
    return static_cast<std::uint16_t>((value >> 8) | (value << 8));
#endif
}

constexpr std::uint32_t byteswap32(std::uint32_t value) noexcept {
#if defined(__cpp_lib_byteswap)
    return std::byteswap(value);
#else
    return ((value & 0x000000FFu) << 24) |
           ((value & 0x0000FF00u) << 8)  |
           ((value & 0x00FF0000u) >> 8)  |
           ((value & 0xFF000000u) >> 24);
#endif
}

inline std::uint16_t be_to_host(std::uint16_t value) noexcept {
#if defined(__cpp_lib_byteswap)
    if constexpr (std::endian::native == std::endian::little)
        return byteswap16(value);
    else
        return value;
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return byteswap16(value);
#else
    return value;
#endif
}

inline std::uint32_t be_to_host(std::uint32_t value) noexcept {
#if defined(__cpp_lib_byteswap)
    if constexpr (std::endian::native == std::endian::little)
        return byteswap32(value);
    else
        return value;
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return byteswap32(value);
#else
    return value;
#endif
}

inline std::uint16_t host_to_be(std::uint16_t value) noexcept {
    return be_to_host(value);
}

inline std::uint32_t host_to_be(std::uint32_t value) noexcept {
    return be_to_host(value);
}

} // namespace common
