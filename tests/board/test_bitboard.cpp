#include <gtest/gtest.h>
#include "board/bitboard.h"
#include "board/square.h"

// TEST(SuiteName, TestName) — SuiteName groups related tests together.
// Pick a SuiteName that matches what you're testing (the type or module),
// and a TestName that reads like a plain-English description of the behavior.

// ── Square-to-bit mapping ─────────────────────────────────────────────────────
// The LSB convention: a1 = bit 0, h8 = bit 63.
// These tests pin that contract so a future refactor can't silently break it.

TEST(BitboardTest, A1IsBit0) {
    EXPECT_EQ(setBit(0, Square::a1), uint64_t(1));
}

TEST(BitboardTest, H8IsBit63) {
    EXPECT_EQ(setBit(0, Square::h8), uint64_t(1) << 63);
}

TEST(BitboardTest, E4IsCorrectBit) {
    // e4 = file e (index 4) + rank 4 (index 3) * 8 = 28
    EXPECT_EQ(setBit(0, Square::e4), uint64_t(1) << 28);
}

// ── Set / clear / test ────────────────────────────────────────────────────────
// Each test covers one operation in isolation.
// Keeping them separate means a failing test name tells you exactly what broke.

TEST(BitboardTest, SetBitTurnsOnSquare) {
    Bitboard bb = 0;
    bb = setBit(bb, Square::d5);
    EXPECT_TRUE(isBitSet(bb, Square::d5));
}

TEST(BitboardTest, SetBitDoesNotAffectOtherSquares) {
    Bitboard bb = 0;
    bb = setBit(bb, Square::d5);
    EXPECT_FALSE(isBitSet(bb, Square::e5));
}

TEST(BitboardTest, ClearBitTurnsOffSquare) {
    Bitboard bb = ~uint64_t(0); // all bits set
    bb = clearBit(bb, Square::d5);
    EXPECT_FALSE(isBitSet(bb, Square::d5));
}

TEST(BitboardTest, ClearBitDoesNotAffectOtherSquares) {
    Bitboard bb = ~uint64_t(0);
    bb = clearBit(bb, Square::d5);
    EXPECT_TRUE(isBitSet(bb, Square::e5));
}

// ── Popcount ──────────────────────────────────────────────────────────────────

TEST(BitboardTest, PopcountOfZeroIsZero) {
    EXPECT_EQ(countBits(uint64_t(0)), uint64_t(0));
}

TEST(BitboardTest, PopcountOfStartingPawns) {
    // Both ranks of pawns = 16 set bits
    Bitboard pawns = 0x00FF00000000FF00ULL;
    EXPECT_EQ(countBits(pawns), uint64_t(16));
}

TEST(BitboardTest, PopcountOfAllBitsIs64) {
    EXPECT_EQ(countBits(~uint64_t(0)), uint64_t(64));
}

// ── LSB (least significant bit) ───────────────────────────────────────────────
// getLSBIndex() returns the lowest set bit's square — used in move generation
// to iterate over pieces: while (bb) { sq = popLSB(bb); ... }

TEST(BitboardTest, LsbOfA1IsA1) {
    EXPECT_EQ(getLSBIndex(setBit(0, Square::a1)), Square::a1);
}

TEST(BitboardTest, LsbPicksLowestBit) {
    // Set a3 and h8 — getLSBIndex should return a3 (the lower index)
    Bitboard bb = setBit(setBit(0, Square::a3), Square::h8);
    EXPECT_EQ(getLSBIndex(bb), Square::a3);
}

TEST(BitboardTest, LsbOfEmptyBoardIsNoSquare) {
    EXPECT_EQ(getLSBIndex(0), Square::NoSquare);
}

// ── PopLSB ────────────────────────────────────────────────────────────────────
// popLSB() both returns and removes the lowest bit — the standard loop pattern.

TEST(BitboardTest, PopLsbReturnsAndClearsLowestBit) {
    Bitboard bb = setBit(setBit(0, Square::a1), Square::h8);
    Square sq = popLSB(bb);
    EXPECT_EQ(sq, Square::a1);
    EXPECT_FALSE(isBitSet(bb, Square::a1)); // a1 was cleared
    EXPECT_TRUE(isBitSet(bb, Square::h8));  // h8 still set
}
