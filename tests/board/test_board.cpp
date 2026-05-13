#include <gtest/gtest.h>
#include "board/chess_board.h"
#include "board/bitboard.h"

// ── T-004: Board mutation — occupancy consistency ─────────────────────────────

TEST(BoardTest, DefaultConstructorOccupancyConsistent) {
    ChessBoard board;
    Bitboard expectedWhite = board.whitePawns | board.whiteKnights | board.whiteBishops
                           | board.whiteRooks | board.whiteQueens  | board.whiteKing;
    Bitboard expectedBlack = board.blackPawns | board.blackKnights | board.blackBishops
                           | board.blackRooks | board.blackQueens  | board.blackKing;

    EXPECT_EQ(board.whitePieces, expectedWhite);
    EXPECT_EQ(board.blackPieces, expectedBlack);
    EXPECT_EQ(board.allPieces,   expectedWhite | expectedBlack);
}

TEST(BoardTest, MakeMoveUpdatesPiecePosition) {
    ChessBoard board;
    Move m = Move::make(Square::e2, Square::e4, MoveFlag::DoublePush);
    board.makeMove(m);

    EXPECT_FALSE(isBitSet(board.whitePawns, Square::e2));
    EXPECT_TRUE(isBitSet(board.whitePawns, Square::e4));
}

TEST(BoardTest, MakeMoveOccupancyRemainsConsistent) {
    ChessBoard board;
    board.makeMove(Move::make(Square::e2, Square::e4, MoveFlag::DoublePush));

    Bitboard expectedWhite = board.whitePawns | board.whiteKnights | board.whiteBishops
                           | board.whiteRooks | board.whiteQueens  | board.whiteKing;
    Bitboard expectedBlack = board.blackPawns | board.blackKnights | board.blackBishops
                           | board.blackRooks | board.blackQueens  | board.blackKing;

    EXPECT_EQ(board.whitePieces, expectedWhite);
    EXPECT_EQ(board.blackPieces, expectedBlack);
    EXPECT_EQ(board.allPieces,   expectedWhite | expectedBlack);
}

TEST(BoardTest, CaptureRemovesOpponentPiece) {
    // Set up a position where white can immediately capture (scholar's mate setup via FEN)
    ChessBoard board;
    // Place a black pawn on e4, white pawn on d5 to capture
    board.loadFEN("8/8/8/3P4/4p3/8/8/8 w - - 0 1");
    board.printBoard();
    board.makeMove(Move::make(Square::d5, Square::e4, MoveFlag::Capture));
    board.printBoard();
    std::cout << "White pieces bitboard: " << std::hex << board.whitePieces << std::endl;
    std::cout << "Black pieces bitboard: " << std::hex << board.blackPieces << std::endl;
    EXPECT_FALSE(isBitSet(board.blackPawns, Square::e4));
    EXPECT_TRUE(isBitSet(board.whitePawns, Square::e4));
    EXPECT_EQ(board.allPieces, board.whitePieces | board.blackPieces);
}

// ── T-005: FEN round-trip ─────────────────────────────────────────────────────

TEST(FenTest, StartPosition) {
    const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, Kiwipete) {
    const std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, EnPassantSquarePreserved) {
    const std::string fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, NoCastlingRights) {
    const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, SideToMoveBlack) {
    const std::string fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.sideToMove, Color::Black);
}

// ── FEN validation — these document what loadFEN must reject ──────────────────
// All tests below will FAIL until the corresponding validation is added to loadFEN.

TEST(FenTest, InvalidPieceCharacterThrows) {
    ChessBoard board;
    // 'X' is not a valid piece character
    EXPECT_THROW(
        board.loadFEN("rnbqkXnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
        std::invalid_argument
    );
}

TEST(FenTest, InvalidSideToMoveThrows) {
    ChessBoard board;
    EXPECT_THROW(
        board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1"),
        std::invalid_argument
    );
}

TEST(FenTest, InvalidEnPassantFileThrows) {
    ChessBoard board;
    // 'i' is out of range a–h
    EXPECT_THROW(
        board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq i3 0 1"),
        std::invalid_argument
    );
}

TEST(FenTest, InvalidEnPassantRankThrows) {
    ChessBoard board;
    // rank 9 is out of range 1–8
    EXPECT_THROW(
        board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e9 0 1"),
        std::invalid_argument
    );
}

TEST(FenTest, RankTooShortThrows) {
    ChessBoard board;
    // rank 1 only has 7 squares (PPPPPPP instead of 8)
    EXPECT_THROW(
        board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPP/RNBQKBNR w KQkq - 0 1"),
        std::invalid_argument
    );
}

// ── toFEN castling string ordering ───────────────────────────────────────────
// These catch the else-if bug: all active castling rights must appear, not just the first.

TEST(FenTest, ToFenPrintsKqCastling) {
    const std::string fen = "r3k2r/8/8/8/8/8/8/R3K2R w Kq - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, ToFenPrintsQkCastling) {
    const std::string fen = "r3k2r/8/8/8/8/8/8/R3K2R w Qk - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, ToFenPrintsKQCastling) {
    const std::string fen = "r3k2r/8/8/8/8/8/8/R3K2R w KQ - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, ToFenPrintskqCastling) {
    const std::string fen = "r3k2r/8/8/8/8/8/8/R3K2R w kq - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
}

TEST(FenTest, AllCastlingRightsPreserved) {
    const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.toFEN(), fen);
    EXPECT_EQ(board.castlingRights, 0b1111);
}

TEST(FenTest, HalfmoveAndFullmovePreserved) {
    const std::string fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 5 42";
    ChessBoard board;
    board.loadFEN(fen);
    EXPECT_EQ(board.halfmoveClock,  5);
    EXPECT_EQ(board.fullmoveNumber, 42);
    EXPECT_EQ(board.toFEN(), fen);
}

// ── Additional board mutation tests ───────────────────────────────────────────

TEST(BoardTest, StartPositionHasThirtyTwoPieces) {
    ChessBoard board;
    EXPECT_EQ(static_cast<int>(countBits(board.allPieces)), 32);
}

TEST(BoardTest, LoadFenOccupancyConsistent) {
    ChessBoard board;
    board.loadFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    Bitboard expectedWhite = board.whitePawns | board.whiteKnights | board.whiteBishops
                           | board.whiteRooks | board.whiteQueens  | board.whiteKing;
    Bitboard expectedBlack = board.blackPawns | board.blackKnights | board.blackBishops
                           | board.blackRooks | board.blackQueens  | board.blackKing;
    EXPECT_EQ(board.whitePieces, expectedWhite);
    EXPECT_EQ(board.blackPieces, expectedBlack);
    EXPECT_EQ(board.allPieces,   expectedWhite | expectedBlack);
}

TEST(BoardTest, MakeMoveFlipsSideToMove) {
    ChessBoard board;
    ASSERT_EQ(board.sideToMove, Color::White);
    board.makeMove(Move::make(Square::e2, Square::e4, MoveFlag::DoublePush));
    EXPECT_EQ(board.sideToMove, Color::Black);
    board.makeMove(Move::make(Square::e7, Square::e5, MoveFlag::DoublePush));
    EXPECT_EQ(board.sideToMove, Color::White);
}

TEST(BoardTest, MakeMoveDoublePushSetsEnPassantSquare) {
    ChessBoard board;
    board.makeMove(Move::make(Square::e2, Square::e4, MoveFlag::DoublePush));
    // En passant target is the square the pawn skipped over (e3)
    EXPECT_EQ(board.enPassantSquare, Square::e3);
}

TEST(BoardTest, MakeMoveEnPassantRemovesCapturedPawn) {
    ChessBoard board;
    // White pawn on e5, black pawn just double-pushed to d5 — en passant target is d6
    board.loadFEN("8/8/8/3pP3/8/8/8/8 w - d6 0 1");
    board.makeMove(Move::make(Square::e5, Square::d6, MoveFlag::EnPassant));
    EXPECT_FALSE(isBitSet(board.blackPawns, Square::d5)); // captured pawn removed
    EXPECT_TRUE(isBitSet(board.whitePawns,  Square::d6)); // white pawn moved
}

TEST(BoardTest, MakeMoveKingsideCastleMovesRook) {
    ChessBoard board;
    // Pieces cleared between king and rook for kingside castle
    board.loadFEN("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    board.makeMove(Move::make(Square::e1, Square::g1, MoveFlag::KingCastle));
    EXPECT_TRUE(isBitSet(board.whiteKing,  Square::g1));
    EXPECT_TRUE(isBitSet(board.whiteRooks, Square::f1));
    EXPECT_FALSE(isBitSet(board.whiteRooks, Square::h1));
}

TEST(BoardTest, MakeMovePromotionAddsPromotedPiece) {
    ChessBoard board;
    board.loadFEN("8/4P3/8/8/8/8/8/8 w - - 0 1");
    board.makeMove(Move::make(Square::e7, Square::e8, MoveFlag::QueenPromotion));
    EXPECT_TRUE(isBitSet(board.whiteQueens, Square::e8));
    EXPECT_FALSE(isBitSet(board.whitePawns, Square::e8));
    EXPECT_FALSE(isBitSet(board.whitePawns, Square::e7));
}

TEST(BoardTest, WhiteKingMoveClearsWhiteCastlingRights) {
    ChessBoard board;
    board.loadFEN("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    board.makeMove(Move::make(Square::e1, Square::f1, MoveFlag::Quiet));
    // Both white castling bits (bit 0 = kingside, bit 1 = queenside) should be cleared
    EXPECT_FALSE(board.castlingRights & 0b0001);
    EXPECT_FALSE(board.castlingRights & 0b0010);
    // Black castling rights should be untouched
    EXPECT_TRUE(board.castlingRights & 0b0100);
    EXPECT_TRUE(board.castlingRights & 0b1000);
}

TEST(BoardTest, CapturingRookOnOriginalSquareClearsCastlingRight) {
    ChessBoard board;
    // Black queen on g2 can capture white rook on h1 (white kingside castling)
    board.loadFEN("4k3/8/8/8/8/8/6q1/R3K2R b KQkq - 0 1");
    board.makeMove(Move::make(Square::g2, Square::h1, MoveFlag::Capture));
    EXPECT_FALSE(board.castlingRights & 0b0001); // white kingside gone
    EXPECT_TRUE(board.castlingRights  & 0b0010); // white queenside intact
}

TEST(BoardTest, CapturingRookAwayFromOriginalSquareDoesNotAffectCastlingRight) {
    ChessBoard board;
    // White rook has moved from h1 to h4; captured there. h1 castling right should
    // already be gone (rook moved), so capturing it elsewhere changes nothing new.
    board.loadFEN("4k3/8/8/8/7R/8/6q1/R3K3 b Q - 0 1");
    board.makeMove(Move::make(Square::g2, Square::h4, MoveFlag::Capture));
    EXPECT_FALSE(board.castlingRights & 0b0001); // was already gone before capture
    EXPECT_TRUE(board.castlingRights  & 0b0010); // white queenside still intact
}

TEST(BoardTest, EnPassantSquareClearedAfterNonDoublePush) {
    ChessBoard board;
    // Position has an en passant square set
    board.loadFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    ASSERT_EQ(board.enPassantSquare, Square::e3);
    // Black makes a quiet knight move — en passant square must be cleared
    board.makeMove(Move::make(Square::g8, Square::f6, MoveFlag::Quiet));
    EXPECT_EQ(board.enPassantSquare, Square::NoSquare);
}
