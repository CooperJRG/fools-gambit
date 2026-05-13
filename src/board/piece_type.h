#pragma once

#include <cstdint>

enum class PieceType : uint8_t {
    Nil = 0,
    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6
};
