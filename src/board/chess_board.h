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

    // Update castling rights based on the move made. This function will check if the move involves a king or rook and update the castling rights accordingly.
    void updateCastlingRights(const Square from, const Square to);

    // Get a pointer to the bitboard that contains the piece on the given square. This is useful for move generation and making moves, as it allows us to quickly find and update the piece on a square.
    Bitboard* getBoardForSquare(Square square, Color color);

    // Handle captures. This function will check if the move is a capture and update the board state accordingly by clearing the captured piece from its bitboard.
    bool handleCapture(const Square to, const MoveFlag flag);

    // Handle castling. This function will check if the move is a castling move and update the board state accordingly by moving the rook to the appropriate square.
    void handleCastling(const Square to, const MoveFlag flag);

    // Place the moving piece on the destination square. This function will also handle promotions by placing the promoted piece instead of the pawn.
    void placeMovingPiece(const Square to, const MoveFlag flag, Bitboard* movingBoard);

    // Update the game state after a move is made. This function will handle updating castling rights, en passant square, halfmove clock, and fullmove number based on the move made.
    void updateGameState(const Move &move);

    // Move from UCI move string (e.g., "e2e4") to Move struct. This will be useful for parsing moves from the user interface or from a PGN file.
    static Move parseMove(const std::string &uciMove, const ChessBoard &board);

    // Convert a Move struct to a UCI move string (e.g., "e2e4"). This will be useful for outputting moves in a format that can be easily read by the user interface or included in a PGN file.
    static std::string moveToUCI(const Move &move);
};
