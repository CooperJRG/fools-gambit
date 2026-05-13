#include "chess_board.h"
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

    sideToMove = (sideToMoveStr == "w") ? Color::White : Color::Black;

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
            enPassantSquare = static_cast<Square>(rank * 8 + file);
        }
    }

    halfmoveClock = std::stoi(halfmoveClockStr);
    fullmoveNumber = std::stoi(fullmoveNumberStr);

    int rank = 7;
    int file = 0;
    for (char c : piecePlacement) {
        if (c == '/') {
            rank--;
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
            }
            file++;
        }
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
    } else if (castlingRights & 0b0001) {
        fen += "K";
    } else if (castlingRights & 0b0010) {
        fen += "Q";
    } else if (castlingRights & 0b0100) {
        fen += "k";
    } else if (castlingRights & 0b1000) {
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
