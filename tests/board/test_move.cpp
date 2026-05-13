#include <gtest/gtest.h>
#include "movegen/move.h"
#include "movegen/flag.h"
#include "board/chess_board.h"

// ── T-006: Move encode / decode ───────────────────────────────────────────────
// Move packs from (6 bits), to (6 bits), flag (4 bits) into a uint16_t.
// These tests pin that layout so a future change to the encoding can't silently break it.

TEST(MoveTest, BasicFromTo) {
    Move m = Move::make(Square::e2, Square::e4, MoveFlag::DoublePush);
    EXPECT_EQ(m.from(), Square::e2);
    EXPECT_EQ(m.to(),   Square::e4);
    EXPECT_EQ(m.flag(), MoveFlag::DoublePush);
}

TEST(MoveTest, QuietMove) {
    Move m = Move::make(Square::g1, Square::f3, MoveFlag::Quiet);
    EXPECT_EQ(m.from(), Square::g1);
    EXPECT_EQ(m.to(),   Square::f3);
    EXPECT_EQ(m.flag(), MoveFlag::Quiet);
}

TEST(MoveTest, CaptureFlag) {
    Move m = Move::make(Square::d5, Square::e6, MoveFlag::Capture);
    EXPECT_EQ(m.flag(), MoveFlag::Capture);
}

TEST(MoveTest, EnPassantFlag) {
    Move m = Move::make(Square::e5, Square::d6, MoveFlag::EnPassant);
    EXPECT_EQ(m.from(), Square::e5);
    EXPECT_EQ(m.to(),   Square::d6);
    EXPECT_EQ(m.flag(), MoveFlag::EnPassant);
}

// ── Promotion flags ───────────────────────────────────────────────────────────

TEST(MoveTest, QueenPromotion) {
    Move m = Move::make(Square::e7, Square::e8, MoveFlag::QueenPromotion);
    EXPECT_EQ(m.from(), Square::e7);
    EXPECT_EQ(m.to(),   Square::e8);
    EXPECT_EQ(m.flag(), MoveFlag::QueenPromotion);
}

TEST(MoveTest, KnightPromotion) {
    Move m = Move::make(Square::a7, Square::a8, MoveFlag::KnightPromotion);
    EXPECT_EQ(m.flag(), MoveFlag::KnightPromotion);
}

TEST(MoveTest, QueenPromotionCapture) {
    Move m = Move::make(Square::e7, Square::d8, MoveFlag::QueenPromotionCapture);
    EXPECT_EQ(m.flag(), MoveFlag::QueenPromotionCapture);
}

// ── Castling flags ────────────────────────────────────────────────────────────

TEST(MoveTest, KingsideCastle) {
    Move m = Move::make(Square::e1, Square::g1, MoveFlag::KingCastle);
    EXPECT_EQ(m.from(), Square::e1);
    EXPECT_EQ(m.to(),   Square::g1);
    EXPECT_EQ(m.flag(), MoveFlag::KingCastle);
}

TEST(MoveTest, QueensideCastle) {
    Move m = Move::make(Square::e1, Square::c1, MoveFlag::QueenCastle);
    EXPECT_EQ(m.flag(), MoveFlag::QueenCastle);
}

// ── UCI round-trip ────────────────────────────────────────────────────────────

TEST(MoveTest, UCIRoundTripQuiet) {
    ChessBoard board;
    board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move m = ChessBoard::parseMove("e2e4", board);
    EXPECT_EQ(ChessBoard::moveToUCI(m), "e2e4");
}

TEST(MoveTest, UCIRoundTripPromotion) {
    ChessBoard board;
    board.loadFEN("8/4P3/8/8/8/8/8/8 w - - 0 1");
    Move m = ChessBoard::parseMove("e7e8q", board);
    EXPECT_EQ(ChessBoard::moveToUCI(m), "e7e8q");
}

TEST(MoveTest, UCIRoundTripKingsideCastle) {
    ChessBoard board;
    board.loadFEN("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    Move m = ChessBoard::parseMove("e1g1", board);
    EXPECT_EQ(m.flag(), MoveFlag::KingCastle);
    EXPECT_EQ(ChessBoard::moveToUCI(m), "e1g1");
}

// Promotion to an empty square must not be inferred as a capture.
// parseMove is board-aware, so this documents the intended behaviour of the
// board-context check: destination empty → non-capture promotion flag.
TEST(MoveTest, ParseUciPromotionToEmptySquareIsNotCapture) {
    ChessBoard board;
    board.loadFEN("8/4P3/8/8/8/8/8/8 w - - 0 1"); // e8 is empty
    Move m = ChessBoard::parseMove("e7e8q", board);
    EXPECT_EQ(m.flag(), MoveFlag::QueenPromotion);
}

TEST(MoveTest, ParseUciPromotionToOccupiedSquareIsCapture) {
    ChessBoard board;
    board.loadFEN("3r4/4P3/8/8/8/8/8/8 w - - 0 1"); // d8 has a black rook
    Move m = ChessBoard::parseMove("e7d8q", board);
    EXPECT_EQ(m.flag(), MoveFlag::QueenPromotionCapture);
}
