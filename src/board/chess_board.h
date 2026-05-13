#pragma once

#include "bitboard.h"
#include "color.h"
#include "square.h"

#include <cstdint>
#include <string>
#include <sstream>

struct ChessBoard {
    Bitboard whitePawns;
    Bitboard whiteKnights;
    Bitboard whiteBishops;
    Bitboard whiteRooks;
    Bitboard whiteQueens;
    Bitboard whiteKing;

    Bitboard blackPawns;
    Bitboard blackKnights;
    Bitboard blackBishops;
    Bitboard blackRooks;
    Bitboard blackQueens;
    Bitboard blackKing;

    Bitboard whitePieces;
    Bitboard blackPieces;
    Bitboard allPieces;

    Color sideToMove;

    // Bit layout:
    // bit 0 = white kingside
    // bit 1 = white queenside
    // bit 2 = black kingside
    // bit 3 = black queenside
    uint8_t castlingRights;

    // -1 means no en passant square.
    // Otherwise stores square index 0-63.
    Square enPassantSquare;

    uint16_t halfmoveClock;
    uint16_t fullmoveNumber;

    ChessBoard();

    void updateOccupancy();

    void printBoard() const;

    // FEN Parser
    void loadFEN(const std::string &fen);

    std::string toFEN() const;
};
