#include "chess_board.h"
#include "board/bitboard.h"
#include "board/square.h"
#include "board/color.h"
#include "board/piece_type.h"
#include "movegen/move.h"
#include <vector>
#include <sstream>

ChessBoard::ChessBoard() {
    whitePawns   = 0x000000000000FF00ULL;
    whiteKnights = 0x0000000000000042ULL;
    whiteBishops = 0x0000000000000024ULL;
    whiteRooks   = 0x0000000000000081ULL;
    whiteQueens  = 0x0000000000000008ULL;
    whiteKing    = 0x0000000000000010ULL;

    blackPawns   = 0x00FF000000000000ULL;
    blackKnights = 0x4200000000000000ULL;
    blackBishops = 0x2400000000000000ULL;
    blackRooks   = 0x8100000000000000ULL;
    blackQueens  = 0x0800000000000000ULL;
    blackKing    = 0x1000000000000000ULL;

    updateOccupancy();

    sideToMove = Color::White;

    castlingRights = 0b1111;

    enPassantSquare = Square::NoSquare;

    halfmoveClock = 0;
    fullmoveNumber = 1;
}

void ChessBoard::updateOccupancy() {
    whitePieces =
        whitePawns |
        whiteKnights |
        whiteBishops |
        whiteRooks |
        whiteQueens |
        whiteKing;

    blackPieces =
        blackPawns |
        blackKnights |
        blackBishops |
        blackRooks |
        blackQueens |
        blackKing;

    allPieces = whitePieces | blackPieces;
}

void ChessBoard::printBoard() const {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            Square square = static_cast<Square>(rank * 8 + file);
            char pieceChar = '.';

            if (isBitSet(whitePawns, square)) pieceChar = 'P';
            else if (isBitSet(whiteKnights, square)) pieceChar = 'N';
            else if (isBitSet(whiteBishops, square)) pieceChar = 'B';
            else if (isBitSet(whiteRooks, square)) pieceChar = 'R';
            else if (isBitSet(whiteQueens, square)) pieceChar = 'Q';
            else if (isBitSet(whiteKing, square)) pieceChar = 'K';
            else if (isBitSet(blackPawns, square)) pieceChar = 'p';
            else if (isBitSet(blackKnights, square)) pieceChar = 'n';
            else if (isBitSet(blackBishops, square)) pieceChar = 'b';
            else if (isBitSet(blackRooks, square)) pieceChar = 'r';
            else if (isBitSet(blackQueens, square)) pieceChar = 'q';
            else if (isBitSet(blackKing, square)) pieceChar = 'k';

            std::cout << pieceChar << " ";
        }
        std::cout << std::endl;
    }
}

void ChessBoard::loadFEN(const std::string &fen) {
    // Clear the board
    whitePawns = whiteKnights = whiteBishops = whiteRooks = whiteQueens = whiteKing = 0;
    blackPawns = blackKnights = blackBishops = blackRooks = blackQueens = blackKing = 0;
    castlingRights = 0;

    // Traverse the FEN string and set pieces accordingly
    // FEN format: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    // We can split the FEN string into its components using spaces as delimiters. The first part is the piece placement, the second part is the side to move, the third part is castling rights, the fourth part is en passant target square, the fifth part is halfmove clock, and the sixth part is fullmove number.
    std::stringstream ss(fen);
    std::string fenPiece;
    std::vector<std::string> fenParts;

    while (std::getline(ss, fenPiece, ' ')) {
        fenParts.push_back(fenPiece);
    }
    // Validate that we have exactly 6 parts in the FEN string
    if (fenParts.size() != 6) {
        throw std::invalid_argument("Invalid FEN string: Incorrect number of fields");
    }
    std::string piecePlacement = fenParts[0];
    std::string sideToMoveStr = fenParts[1];
    std::string castlingRightsStr = fenParts[2];
    std::string enPassantStr = fenParts[3];
    std::string halfmoveClockStr = fenParts[4];
    std::string fullmoveNumberStr = fenParts[5];

    switch (sideToMoveStr[0]) {
        case 'w': sideToMove = Color::White; break;
        case 'b': sideToMove = Color::Black; break;
        default:
            throw std::invalid_argument("Invalid FEN string: Invalid side to move");
    }

    for (char c : castlingRightsStr) {
        switch (c) {
            case 'K': castlingRights |= 0b0001; break;
            case 'Q': castlingRights |= 0b0010; break;
            case 'k': castlingRights |= 0b0100; break;
            case 'q': castlingRights |= 0b1000; break;
        }
    }

    if (enPassantStr == "-") {
        enPassantSquare = Square::NoSquare;
    } else {
        char c = enPassantStr[0];
        if (c >= 'a' && c <= 'h') {
            int file = c - 'a';
            int rank = enPassantStr[1] - '1';
            if (rank != 2 && rank != 5) {
                throw std::invalid_argument("Invalid FEN string: En passant square must be on rank 3 or 6");
            }
            enPassantSquare = static_cast<Square>(rank * 8 + file);
        } else {
            throw std::invalid_argument("Invalid FEN string: Invalid en passant square");
        }
    }

    halfmoveClock = std::stoi(halfmoveClockStr);
    fullmoveNumber = std::stoi(fullmoveNumberStr);

    int rank = 7;
    int file = 0;
    for (char c : piecePlacement) {
        if (c == '/') {
            rank--;
            if (file != 8) {
                throw std::invalid_argument("Invalid FEN string: Incorrect number of squares in a rank");
            }
            file = 0;
        } else if (isdigit(c)) {
            file += c - '0'; // Skip empty squares
            if (file > 8) {
                throw std::invalid_argument("Invalid FEN string: Too many squares in a rank");
            }
        } // Only case left should be for pieces
        else {
            Square square = static_cast<Square>(rank * 8 + file);
            switch (c) {
                case 'P': whitePawns = setBit(whitePawns, square); break;
                case 'N': whiteKnights = setBit(whiteKnights, square); break;
                case 'B': whiteBishops = setBit(whiteBishops, square); break;
                case 'R': whiteRooks = setBit(whiteRooks, square); break;
                case 'Q': whiteQueens = setBit(whiteQueens, square); break;
                case 'K': whiteKing = setBit(whiteKing, square); break;
                case 'p': blackPawns = setBit(blackPawns, square); break;
                case 'n': blackKnights = setBit(blackKnights, square); break;
                case 'b': blackBishops = setBit(blackBishops, square); break;
                case 'r': blackRooks = setBit(blackRooks, square); break;
                case 'q': blackQueens = setBit(blackQueens, square); break;
                case 'k': blackKing = setBit(blackKing, square); break;
                default:
                    throw std::invalid_argument("Invalid FEN string: Unknown piece type");
            }
            file++;
        }
    }
    if (rank != 0 || file != 8) {
        throw std::invalid_argument("Invalid FEN string: Incorrect number of ranks or squares");
    }
    updateOccupancy();
}

std::string ChessBoard::toFEN() const {
    std::string fen;

    for (int rank = 7; rank >= 0; --rank) {
        int emptyCount = 0;
        for (int file = 0; file < 8; ++file) {
            Square square = static_cast<Square>(rank * 8 + file);
            char pieceChar = '.';

            if (isBitSet(whitePawns, square)) pieceChar = 'P';
            else if (isBitSet(whiteKnights, square)) pieceChar = 'N';
            else if (isBitSet(whiteBishops, square)) pieceChar = 'B';
            else if (isBitSet(whiteRooks, square)) pieceChar = 'R';
            else if (isBitSet(whiteQueens, square)) pieceChar = 'Q';
            else if (isBitSet(whiteKing, square)) pieceChar = 'K';
            else if (isBitSet(blackPawns, square)) pieceChar = 'p';
            else if (isBitSet(blackKnights, square)) pieceChar = 'n';
            else if (isBitSet(blackBishops, square)) pieceChar = 'b';
            else if (isBitSet(blackRooks, square)) pieceChar = 'r';
            else if (isBitSet(blackQueens, square)) pieceChar = 'q';
            else if (isBitSet(blackKing, square)) pieceChar = 'k';

            if (pieceChar == '.') {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += pieceChar;
            }
        }
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        if (rank > 0) {
            fen += '/';
        }
    }

    fen += ' ';
    fen += (sideToMove == Color::White) ? 'w' : 'b';
    fen += ' ';

    if (castlingRights == 0) {
        fen += '-';
    }  if (castlingRights & 0b0001) {
        fen += "K";
    }  if (castlingRights & 0b0010) {
        fen += "Q";
    }  if (castlingRights & 0b0100) {
        fen += "k";
    }  if (castlingRights & 0b1000) {
        fen += "q";
    }

    fen += ' ';
    if (enPassantSquare == Square::NoSquare) {
        fen += '-';
    } else {
        int file = static_cast<int>(enPassantSquare) % 8;
        int rank = static_cast<int>(enPassantSquare) / 8;
        fen += static_cast<char>('a' + file);
        fen += static_cast<char>('1' + rank);
    }
    fen += ' ';
    fen += std::to_string(halfmoveClock);
    fen += ' ';
    fen += std::to_string(fullmoveNumber);

    return fen;
}

void ChessBoard::makeMove(const Move& move) {
    Square from = move.from();
    Square to = move.to();
    MoveFlag flag = move.flag();

    Bitboard *movingPieceBoard = getBoardForSquare(from, sideToMove);
    if (movingPieceBoard == nullptr) {
        throw std::invalid_argument("Invalid move: No piece on the source square");
    }
    updateCastlingRights(from, to);

    *movingPieceBoard = clearBit(*movingPieceBoard, from);

    if (!handleCapture(to, flag)) {
        handleCastling(to, flag); // Small optimization to avoid checking for castling if a capture was made, since captures can't be castling moves.
    } else {
        // If a capture was made, we need to reset the halfmove clock to 0, since captures reset the halfmove clock.
        halfmoveClock = -1; // We will increment it to 0 in updateGameState() after making the move.
    }

    placeMovingPiece(to, flag, movingPieceBoard);

    // Update game state (castling rights, en passant square, halfmove clock, fullmove number)
    updateGameState(move);

    updateOccupancy();

    // Switch the side to move
    sideToMove = !sideToMove;
}

Bitboard* ChessBoard::getBoardForSquare(Square square, Color color) {
    if (color == Color::White) {
        if (isBitSet(whitePawns, square))   return &whitePawns;
        if (isBitSet(whiteKnights, square)) return &whiteKnights;
        if (isBitSet(whiteBishops, square)) return &whiteBishops;
        if (isBitSet(whiteRooks, square))   return &whiteRooks;
        if (isBitSet(whiteQueens, square))  return &whiteQueens;
        if (isBitSet(whiteKing, square))    return &whiteKing;
    } else {
        if (isBitSet(blackPawns, square))   return &blackPawns;
        if (isBitSet(blackKnights, square)) return &blackKnights;
        if (isBitSet(blackBishops, square)) return &blackBishops;
        if (isBitSet(blackRooks, square))   return &blackRooks;
        if (isBitSet(blackQueens, square))  return &blackQueens;
        if (isBitSet(blackKing, square))    return &blackKing;
    }
    return nullptr;
}

// I want to mark this for potential room for improvement as it adds redundant checks for whether the
// moving piece is a king or rook, since we will already know that from the move generation phase.
// We could potentially optimize this in a couple of ways. TODO
void ChessBoard::updateCastlingRights(const Square from, const Square to) {
    if (castlingRights == 0) {
        return; // No castling rights to update, so we can skip the checks.
    }
    // Check if the moving piece is a king or rook, since we need to update castling rights if either of those pieces move.{
    if (isBitSet(whiteKing, from) || isBitSet(blackKing, from)) {
        // If the king moves, we lose all castling rights for that color.
        if (sideToMove == Color::White) {
            castlingRights &= 0b1100; // Clear white's castling rights
        } else {
            castlingRights &= 0b0011; // Clear black
        }
    } else if (isBitSet(whiteRooks, from) || isBitSet(blackRooks, from) || isBitSet(whiteRooks, to) || isBitSet(blackRooks, to)) {
        // If a rook moves or is captured, we lose the castling right for that rook's side.
        if (sideToMove == Color::White) {
            if (from == Square::h1) {
                castlingRights &= 0b1110; // Clear white kingside
            } else if (from == Square::a1) {
                castlingRights &= 0b1101; // Clear white queenside
            } else if (to == Square::h8) {
                castlingRights &= 0b1011; // Clear black kingside (capture)
            } else if (to == Square::a8) {
                castlingRights &= 0b0111; // Clear black queenside (capture)
            }
        } else {
            if (from == Square::h8) {
                castlingRights &= 0b1011; // Clear black kingside
            } else if (from == Square::a8) {
                castlingRights &= 0b0111; // Clear black queenside
            } else if (to == Square::h1) {
                castlingRights &= 0b1110; // Clear white kingside (capture)
            } else if (to == Square::a1) {
                castlingRights &= 0b1101; // Clear white queenside (capture)
            }
        }
    }
}

bool ChessBoard::handleCapture(const Square to, const MoveFlag flag) {
    // This function will handle the logic for capturing a piece on the destination square.
    // It will check if there is an opponent's piece on the destination square and clear it from the appropriate bitboard.
    //     if (flag == MoveFlag::Capture || (flag >= MoveFlag::KnightPromotionCapture && flag <= MoveFlag::QueenPromotionCapture)) {
    // Clear the captured piece from the opponent's bitboards
    if (flag == MoveFlag::Capture || (flag >= MoveFlag::KnightPromotionCapture && flag <= MoveFlag::QueenPromotionCapture)) {
        // Clear the captured piece from the opponent's bitboards
        Square capturedSquare = to;
        Bitboard *capturedPieceBoard = getBoardForSquare(capturedSquare, !sideToMove);
        if (capturedPieceBoard) {
            *capturedPieceBoard = clearBit(*capturedPieceBoard, capturedSquare);
            return true; // Capture was made
        }
    } else if (flag == MoveFlag::EnPassant) {
        // Clear the captured pawn for en passant
        Square capturedSquare = static_cast<Square>(static_cast<int>(to) + ((sideToMove == Color::White) ? -8 : 8));
        Bitboard *capturedPieceBoard = getBoardForSquare(capturedSquare, !sideToMove);
        if (capturedPieceBoard) {
            *capturedPieceBoard = clearBit(*capturedPieceBoard, capturedSquare);
            return true; // Capture was made
        }
    }
    return false; // No capture was made
}

void ChessBoard::handleCastling(const Square to, const MoveFlag flag) {
    // This function will handle the logic for castling moves. It will move the rook to the appropriate square based on whether it's kingside or queenside castling.
    if (flag == MoveFlag::KingCastle) {
        // Move the rook for kingside castling
        Square rookFrom = (sideToMove == Color::White) ? Square::h1 : Square::h8;
        Square rookTo = (sideToMove == Color::White) ? Square::f1 : Square::f8;
        Bitboard *rookBoard = (sideToMove == Color::White) ? &whiteRooks : &blackRooks;
        *rookBoard = clearBit(*rookBoard, rookFrom);
        *rookBoard = setBit(*rookBoard, rookTo);
    } else if (flag == MoveFlag::QueenCastle) {
        // Move the rook for queenside castling
        Square rookFrom = (sideToMove == Color::White) ? Square::a1 : Square::a8;
        Square rookTo = (sideToMove == Color::White) ? Square::d1 : Square::d8;
        Bitboard *rookBoard = (sideToMove == Color::White) ? &whiteRooks : &blackRooks;
        *rookBoard = clearBit(*rookBoard, rookFrom);
        *rookBoard = setBit(*rookBoard, rookTo);
    }
}

void ChessBoard::placeMovingPiece(const Square to, const MoveFlag flag, Bitboard* movingBoard) {
    // This function will place the moving piece on the destination square. It will also handle promotions by placing the promoted piece instead of the pawn.
    if (flag >= MoveFlag::KnightPromotion && flag <= MoveFlag::QueenPromotionCapture) {
        // Handle promotions by clearing the pawn and setting the promoted piece
        PieceType promotionType = static_cast<PieceType>(static_cast<uint8_t>(flag) - static_cast<uint8_t>(MoveFlag::KnightPromotion) + 2);
        Bitboard *promotionBoard = nullptr;
        if (sideToMove == Color::White) {
            switch (promotionType) {
                case PieceType::Knight: promotionBoard = &whiteKnights; break;
                case PieceType::Bishop: promotionBoard = &whiteBishops; break;
                case PieceType::Rook:   promotionBoard = &whiteRooks; break;
                case PieceType::Queen:  promotionBoard = &whiteQueens; break;
                default: break;
            }
        } else {
            switch (promotionType) {
                case PieceType::Knight: promotionBoard = &blackKnights; break;
                case PieceType::Bishop: promotionBoard = &blackBishops; break;
                case PieceType::Rook:   promotionBoard = &blackRooks; break;
                case PieceType::Queen:  promotionBoard = &blackQueens; break;
                default: break;
            }
        }
        if (promotionBoard) {
            *promotionBoard = setBit(*promotionBoard, to);
        }
    } else {
        *movingBoard = setBit(*movingBoard, to);
    }
}

void ChessBoard::updateGameState(const Move &move) {
    // This function will update the game state after a move is made. It will handle updating castling rights, en passant square, halfmove clock, and fullmove number based on the move made.
    MoveFlag flag = move.flag();

    // Update castling rights if a rook or king moves
    if (flag == MoveFlag::KingCastle || flag == MoveFlag::QueenCastle) {
        if (sideToMove == Color::White) {
            castlingRights &= 0b1100; // Clear white's castling rights
        } else {
            castlingRights &= 0b0011; // Clear black's castling rights
        }
    }

    // If pawn double move, then set en passant square
    if (flag == MoveFlag::DoublePush) {
        enPassantSquare = static_cast<Square>(static_cast<int>(move.to()) + ((sideToMove == Color::White) ? -8 : 8));
    } else {
        enPassantSquare = Square::NoSquare;
    }

    // Update game halfmoveClock
    halfmoveClock++;
    fullmoveNumber += (sideToMove == Color::Black) ? 1 : 0;
}

// Move from UCI move string (e.g., "e2e4") to Move struct. This will be useful for parsing moves from the user interface or from a PGN file.
Move ChessBoard::parseMove(const std::string &uciMove, const ChessBoard &board){
    int fromFile = uciMove[0] - 'a';
    int fromRank = uciMove[1] - '1';
    int toFile   = uciMove[2] - 'a';
    int toRank   = uciMove[3] - '1';

    Square from = static_cast<Square>(fromRank * 8 + fromFile);
    Square to   = static_cast<Square>(toRank   * 8 + toFile);

    MoveFlag flag = MoveFlag::Quiet;

    if (uciMove.size() == 5) {
        switch (uciMove[4]) {
            case 'n': flag = isBitSet(board.allPieces, to) ? MoveFlag::KnightPromotionCapture : MoveFlag::KnightPromotion; break;
            case 'b': flag = isBitSet(board.allPieces, to) ? MoveFlag::BishopPromotionCapture : MoveFlag::BishopPromotion; break;
            case 'r': flag = isBitSet(board.allPieces, to) ? MoveFlag::RookPromotionCapture   : MoveFlag::RookPromotion;   break;
            case 'q': flag = isBitSet(board.allPieces, to) ? MoveFlag::QueenPromotionCapture  : MoveFlag::QueenPromotion;  break;
        }
    } else if (isBitSet(board.allPieces, to)) {
        flag = MoveFlag::Capture;
    } else if (to == board.enPassantSquare && isBitSet(board.whitePawns | board.blackPawns, from)) {
        flag = MoveFlag::EnPassant;
    } else if (isBitSet(board.whitePawns | board.blackPawns, from) && abs(fromRank - toRank) == 2) {
        flag = MoveFlag::DoublePush;
    } else if (isBitSet(board.whiteKing | board.blackKing, from) && abs(fromFile - toFile) == 2) {
        flag = (toFile > fromFile) ? MoveFlag::KingCastle : MoveFlag::QueenCastle;
    }

    return Move::make(from, to, flag);
}

std::string ChessBoard::moveToUCI(const Move &move){
    Square from = move.from();
    Square to   = move.to();

    int fromIdx = static_cast<int>(from);
    int toIdx   = static_cast<int>(to);

    std::string uci;
    uci += static_cast<char>('a' + (fromIdx % 8));
    uci += static_cast<char>('1' + (fromIdx / 8));
    uci += static_cast<char>('a' + (toIdx   % 8));
    uci += static_cast<char>('1' + (toIdx   / 8));

    switch (move.flag()) {
        case MoveFlag::KnightPromotion:        uci += 'n'; break;
        case MoveFlag::BishopPromotion:        uci += 'b'; break;
        case MoveFlag::RookPromotion:          uci += 'r'; break;
        case MoveFlag::QueenPromotion:         uci += 'q'; break;
        case MoveFlag::KnightPromotionCapture: uci += 'n'; break;
        case MoveFlag::BishopPromotionCapture: uci += 'b'; break;
        case MoveFlag::RookPromotionCapture:   uci += 'r'; break;
        case MoveFlag::QueenPromotionCapture:  uci += 'q'; break;
        default: break;
    }

    return uci;
}
