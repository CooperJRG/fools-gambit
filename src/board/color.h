#pragma once

#include <cstdint>

// Enum class for type safety and clarity
enum class Color : uint8_t {
    White,
    Black
};

inline Color operator!(Color c) {
    return c == Color::White ? Color::Black : Color::White;
}
