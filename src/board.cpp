#include "board.h"

Board::Board() {
    for (auto &row : board)
        for (auto &c : row)
            c = Cell::EMPTY;

    board[3][3] = board[4][4] = Cell::WHITE;
    board[3][4] = board[4][3] = Cell::BLACK;
}

bool Board::inBounds(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

Cell Board::getCell(int x, int y) const {
    return board[x][y];
}

bool Board::checkDirection(int x, int y, int dx, int dy, Cell player) const {
    Cell enemy = (player == Cell::BLACK) ? Cell::WHITE : Cell::BLACK;
    int i = x + dx, j = y + dy;
    bool foundEnemy = false;

    while (inBounds(i, j) && board[i][j] == enemy) {
        foundEnemy = true;
        i += dx;
        j += dy;
    }

    return foundEnemy && inBounds(i, j) && board[i][j] == player;
}

bool Board::isValidMove(int x, int y, Cell player) const {
    if (!inBounds(x, y) || board[x][y] != Cell::EMPTY)
        return false;

    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
            if (dx || dy)
                if (checkDirection(x, y, dx, dy, player))
                    return true;

    return false;
}

void Board::flipDirection(int x, int y, int dx, int dy, Cell player) {
    Cell enemy = (player == Cell::BLACK) ? Cell::WHITE : Cell::BLACK;
    int i = x + dx, j = y + dy;

    while (inBounds(i, j) && board[i][j] == enemy) {
        board[i][j] = player;
        i += dx;
        j += dy;
    }
}

bool Board::makeMove(int x, int y, Cell player) {
    if (!isValidMove(x, y, player))
        return false;

    board[x][y] = player;

    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
            if (dx || dy)
                if (checkDirection(x, y, dx, dy, player))
                    flipDirection(x, y, dx, dy, player);

    return true;
}

int Board::count(Cell player) const {
    int c = 0;
    for (auto &row : board)
        for (auto &cell : row)
            if (cell == player)
                c++;
    return c;
}
#include <iostream>

void Board::save(std::ostream& out) const {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            out << static_cast<int>(board[x][y]) << " ";
        }
        out << "\n";
    }
}

void Board::load(std::istream& in) {
    int value;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            in >> value;
            board[x][y] = static_cast<Cell>(value);
        }
    }
}
