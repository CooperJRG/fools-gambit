#pragma once
// This file defines the Move struct, which represents a chess move in the engine.
// It includes the source and destination squares, the piece being moved, and any special move flags (e.g., promotion, castling, en passant).

#include "board/square.h"
#include "movegen/flag.h"
#include <cstdint>

struct Move {
    uint16_t data;

    static Move make(Square from, Square to, MoveFlag flag) {
        return { static_cast<uint16_t>(
            (uint16_t)from | ((uint16_t)to << 6) | ((uint16_t)flag << 12)
        )};
    }

    Square   from() const { return static_cast<Square>  ( data        & 0x3F); }
    Square   to()   const { return static_cast<Square>  ((data >>  6) & 0x3F); }
    MoveFlag flag() const { return static_cast<MoveFlag>((data >> 12) & 0x0F); }
};
