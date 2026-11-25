#pragma once
#include <cstdint>
#include <cstring>
namespace sssv::menu {
inline uint8_t byte(const uint8_t* memory, uint32_t address) {
    return memory[(address & 0x7fffffu) ^ 3u];
}
inline uint16_t half(const uint8_t* memory, uint32_t address) {
    uint16_t value; std::memcpy(&value, memory + ((address & 0x7fffffu) ^ 2u), 2); return value;
}
inline bool title(const uint8_t* memory) {
    if (!memory || byte(memory, 0x80152e90) != 1) return false;
    const auto scene = byte(memory, 0x80299dfc);
    return scene == 3 || scene == 5;
}
inline bool startup(const uint8_t* memory) {
    return memory && (byte(memory, 0x80152e90) == 1 ||
        (byte(memory, 0x80152e90) == 0 && half(memory, 0x803f2d38) == 36));
}
inline bool new_game_intro(const uint8_t* memory) {
    if (!memory || byte(memory, 0x80152e90) != 0 || half(memory, 0x803f2d38) != 1 ||
        half(memory, 0x803f6680) != 0 || half(memory, 0x803e1bc0) != 0 ||
        byte(memory, 0x803c0422) != 0) return false;
    // Bit zero of the native intro-complete word (big-endian low byte).
    return (byte(memory, 0x8023f297) & 1) == 0;
}
inline bool active(const uint8_t* memory) {
    return memory && byte(memory, 0x80152e90) == 0 &&
        (byte(memory, 0x803f2d10) != 0 || half(memory, 0x803f6680) != 0);
}
inline void skip_startup(uint16_t& buttons) {
    if (buttons & 0x8000) buttons = (buttons & ~0x8000u) | 0x1000;
}
inline void advance_title(uint16_t& buttons) {
    if (buttons & 0xc000) buttons = (buttons & ~0xc000u) | 0x1000;
}
}
