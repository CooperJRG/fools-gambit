// Stub main file for fools gambit. This will be the entry point for the application.
#include <iostream>
#include "board/chess_board.h"

int main() {
    ChessBoard board;
    std::cout << "Initial Chess Board Position:" << std::endl;
    board.printBoard();
    return 0;
}
