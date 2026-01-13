#include "ai.h"
#include <cstdlib>
#include <limits>

static Cell opponent(Cell c) {
    return (c == Cell::BLACK) ? Cell::WHITE : Cell::BLACK;
}

/* ---------------- RANDOM ---------------- */

std::pair<int,int> AI::randomMove(const Board& board, Cell player) {
    std::vector<std::pair<int,int>> moves;

    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
            if (board.isValidMove(x, y, player))
                moves.emplace_back(x, y);

    if (moves.empty()) return {-1, -1};
    return moves[rand() % moves.size()];
}

/* ---------------- GREEDY ---------------- */

std::pair<int,int> AI::greedyMove(const Board& board, Cell player) {
    int best = -1;
    std::pair<int,int> bestMove{-1, -1};

    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
            if (board.isValidMove(x, y, player)) {
                Board temp = board;
                temp.makeMove(x, y, player);
                int score = temp.count(player);
                if (score > best) {
                    best = score;
                    bestMove = {x, y};
                }
            }

    return bestMove;
}

/* ---------------- MINIMAX ---------------- */

int AI::minimax(Board board, Cell player, Cell aiPlayer,
                int depth, bool maximizing) {

    if (depth == 0) {
        return board.count(aiPlayer) -
               board.count(opponent(aiPlayer));
    }

    bool hasMove = false;
    int best = maximizing ?
        std::numeric_limits<int>::min() :
        std::numeric_limits<int>::max();

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board.isValidMove(x, y, player)) {
                hasMove = true;
                Board temp = board;
                temp.makeMove(x, y, player);

                int value = minimax(
                    temp,
                    opponent(player),
                    aiPlayer,
                    depth - 1,
                    !maximizing
                );

                if (maximizing)
                    best = std::max(best, value);
                else
                    best = std::min(best, value);
            }
        }
    }

    if (!hasMove)
        return board.count(aiPlayer) -
               board.count(opponent(aiPlayer));

    return best;
}

std::pair<int,int> AI::minimaxMove(const Board& board,
                                   Cell player,
                                   int depth) {
    int bestValue = std::numeric_limits<int>::min();
    std::pair<int,int> bestMove{-1, -1};

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board.isValidMove(x, y, player)) {
                Board temp = board;
                temp.makeMove(x, y, player);

                int value = minimax(
                    temp,
                    opponent(player),
                    player,
                    depth - 1,
                    false
                );

                if (value > bestValue) {
                    bestValue = value;
                    bestMove = {x, y};
                }
            }
        }
    }

    return bestMove;
}
