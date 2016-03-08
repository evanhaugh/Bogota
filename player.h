#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <vector>
using namespace std;

class ScoredMove {
public:
	ScoredMove(Move * move, Side side);
	Move * m;
	int score;
};


class Player {

public:
    Player(Side side);
    ~Player();
    
	Side side;
	Side opponent_side;
	Board * gameboard;
	Move * opponent_move;
    Move *doMove(Move *opponentsMove, int msLeft);
	std::vector<ScoredMove*> * PossibleMoves;

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};


#endif
