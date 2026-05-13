#pragma once
// This file defines the Move struct, which represents a chess move in the engine.
// It includes the source and destination squares, the piece being moved, and any special move flags (e.g., promotion, castling, en passant).

#include "board/square.h"
#include "board/piece.h"

#include <cstdint>

struct Move {
    Square from;
    Square to;
    Piece piece;
    Piece promotion;
    uint8_t flags;
};
