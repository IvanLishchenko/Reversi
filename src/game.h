#pragma once
#include "board.h"
#include "ai.h"
#include <string>

class Game {
public:
    Game(int mode, int difficulty, int humanColor = 1, int difficulty2 = -1);

    Board& getBoard();
    Cell getCurrentPlayer() const;

    bool makeMove(int x, int y);
    void aiMove();

    bool isGameOver() const;
    bool isAI(Cell player) const;
    bool hasValidMoves(Cell player) const;

    int getScoreBlack() const;
    int getScoreWhite() const;

    void save(const std::string& filename) const;
    void load(const std::string& filename);

private:
    Board board;
    Cell current;
    int mode;
    AIDifficulty difficultyBlack;  // AI difficulty for Black
    AIDifficulty difficultyWhite;  // AI difficulty for White
    Cell humanPlayerColor;
    bool gameOver;

    void nextTurn();
    Cell opponent(Cell player) const;
    AIDifficulty getDifficulty(Cell player) const;
};
