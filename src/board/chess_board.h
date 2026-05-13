#pragma once

#include "bitboard.h"
#include "color.h"
#include "square.h"
#include "movegen/move.h"

#include <cstdint>
#include <string>

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

    // Make a move on the board. This will be used by the move generator and search algorithm to update the board state after a move is made.
    void makeMove(const Move &move);
};
