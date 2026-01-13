#pragma once
#include <vector>
#include <iostream>

enum class Cell { EMPTY, BLACK, WHITE };

class Board {
public:
    Board();

    bool isValidMove(int x, int y, Cell player) const;
    bool makeMove(int x, int y, Cell player);

    Cell getCell(int x, int y) const;
    int count(Cell player) const;

    void save(std::ostream& out) const;
    void load(std::istream& in);

private:
    Cell board[8][8];

    bool inBounds(int x, int y) const;
    bool checkDirection(int x, int y, int dx, int dy, Cell player) const;
    void flipDirection(int x, int y, int dx, int dy, Cell player);
};
