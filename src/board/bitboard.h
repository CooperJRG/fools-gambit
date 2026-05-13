#pragma once

#include <cstdint>
#include "square.h"
#include <iostream>

// Alias Bitboard to a 64-bit unsigned integer
using Bitboard = uint64_t;

// Utility functions for bitboard manipulation
// Set a bit at a given square index (0-63)
inline Bitboard setBit(Bitboard board, Square square) {
    return board | (1ULL << static_cast<int>(square));
}

// Clear a bit at a given square index (0-63)
inline Bitboard clearBit(Bitboard board, Square square) {
    return board & ~(1ULL << static_cast<int>(square));
}

// Check if a bit is set at a given square index (0-63)
inline bool isBitSet(Bitboard board, Square square) {
    return (board & (1ULL << static_cast<int>(square))) != 0;
}

// Count the number of set bits in a bitboard (population count) (use built-in function)
inline Square countBits(Bitboard board) {
    return static_cast<Square>(__builtin_popcountll(board));
}

// Get the index of the least significant set bit (LSB) in a bitboard
inline Square getLSBIndex(Bitboard board) {
    // If the board is empty, return -1 to indicate no bits are set
    if (board == 0) {
        return Square::NoSquare;
    }
    return static_cast<Square>(__builtin_ctzll(board));
}

// Pop the least significant set bit (LSB) and return its index
// This function is useful for iterating through all set bits in a bitboard. It clears the LSB and returns its index.
// Remember, each set bit corresponds to a piece on the chessboard, and the index can be used to determine the square of that piece.
inline Square popLSB(Bitboard &board) {
    // If the board is empty, return -1 to indicate no bits are set
    Square lsbIndex = getLSBIndex(board);
    if (lsbIndex == Square::NoSquare) {
        return Square::NoSquare;
    }
    board &= board - 1; // Clear the LSB
    return lsbIndex;
}

// Utility function to print a bitboard (for debugging)
inline void printBitboard(Bitboard board) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            Square square = static_cast<Square>(rank * 8 + file);
            std::cout << (isBitSet(board, square) ? "1 " : ". ");
        }
        std::cout << std::endl;
    }
}
