#include "game.h"

Game::Game(int m, int d, int humanColor, int d2)
    : current(Cell::BLACK), mode(m), gameOver(false) {
    // For Human vs AI: d is the AI difficulty
    // For AI vs AI: d is Black's difficulty, d2 is White's difficulty
    difficultyBlack = static_cast<AIDifficulty>(d);
    difficultyWhite = (d2 >= 0) ? static_cast<AIDifficulty>(d2) : static_cast<AIDifficulty>(d);
    
    // humanColor: 1 = BLACK (first), 2 = WHITE (second)
    humanPlayerColor = (humanColor == 2) ? Cell::WHITE : Cell::BLACK;
}

AIDifficulty Game::getDifficulty(Cell player) const {
    return (player == Cell::BLACK) ? difficultyBlack : difficultyWhite;
}

Cell Game::opponent(Cell player) const {
    return (player == Cell::BLACK) ? Cell::WHITE : Cell::BLACK;
}

bool Game::hasValidMoves(Cell player) const {
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
            if (board.isValidMove(x, y, player))
                return true;
    return false;
}

Board& Game::getBoard() {
    return board;
}

Cell Game::getCurrentPlayer() const {
    return current;
}

bool Game::makeMove(int x, int y) {
    // Block human moves if current player is AI
    if (isAI(current)) return false;
    
    if (board.makeMove(x, y, current)) {
        nextTurn();
        return true;
    }
    return false;
}

void Game::aiMove() {
    if (!isAI(current)) return;

    std::pair<int,int> move;
    AIDifficulty diff = getDifficulty(current);

    if (diff == AIDifficulty::RANDOM)
        move = AI::randomMove(board, current);
    else if (diff == AIDifficulty::GREEDY)
        move = AI::greedyMove(board, current);
    else
        move = AI::minimaxMove(board, current, 3);

    if (move.first != -1)
        board.makeMove(move.first, move.second, current);

    nextTurn();
}

void Game::nextTurn() {
    Cell next = opponent(current);
    
    // Check if next player has moves
    if (hasValidMoves(next)) {
        current = next;
    } 
    // Next player has no moves, check if current player can continue
    else if (hasValidMoves(current)) {
        // Skip next player's turn, current player continues
        // (current stays the same)
    }
    // Neither player has moves - game over
    else {
        gameOver = true;
        current = next;  // Switch anyway for display purposes
    }
}

bool Game::isAI(Cell player) const {
    if (mode == 0) return false;  // Human vs Human
    if (mode == 1) return player != humanPlayerColor;  // Human vs AI
    return true;  // AI vs AI
}

bool Game::isGameOver() const {
    return gameOver;
}

int Game::getScoreBlack() const {
    return board.count(Cell::BLACK);
}

int Game::getScoreWhite() const {
    return board.count(Cell::WHITE);
}
#include <fstream>

void Game::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) return;

    file << static_cast<int>(current) << " " << (gameOver ? 1 : 0) << "\n";
    file << mode << " " << static_cast<int>(difficultyBlack) << " " << static_cast<int>(difficultyWhite) << " " << static_cast<int>(humanPlayerColor) << "\n";
    board.save(file);
}

void Game::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return;

    int cur, go;
    file >> cur >> go;
    current = static_cast<Cell>(cur);
    gameOver = (go == 1);

    int diffB, diffW, hc;
    file >> mode >> diffB >> diffW >> hc;
    difficultyBlack = static_cast<AIDifficulty>(diffB);
    difficultyWhite = static_cast<AIDifficulty>(diffW);
    humanPlayerColor = static_cast<Cell>(hc);

    board.load(file);
}
