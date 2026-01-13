#pragma once
#include "board.h"
#include <utility>

enum class AIDifficulty { RANDOM, GREEDY, MINIMAX };

class AI {
public:
    static std::pair<int,int> randomMove(const Board&, Cell);
    static std::pair<int,int> greedyMove(const Board&, Cell);
    static std::pair<int,int> minimaxMove(const Board&, Cell, int depth = 3);

private:
    static int minimax(Board board, Cell player, Cell aiPlayer,
                       int depth, bool maximizing);
};
