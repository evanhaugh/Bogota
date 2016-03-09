#include "player.h"

// Constructor for ScoredMove type
ScoredMove::ScoredMove(Move * move, Side side, Board * gameboard)
{
	/* I initialize m in this convulted way so that the pointer it is given
	 * can be safely freed in the program to save memory.
	 */
	this->m = new Move(move->x, move->y);
	this->score = 0;
	Board * copiedboard = gameboard->copy();
	copiedboard->doMove(m, side);
	
	// Establish an otherside variable
	Side otherside = BLACK;
	if (side == BLACK)
	{
		otherside = WHITE;
	}
	
	// Determine all possible moves for opponent's piece.
	
	std::vector<Move *> OpponentMoves;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Move * testmove = new Move(x,y);
			if(gameboard->checkMove(testmove, otherside))
			{
				OpponentMoves.push_back(new Move(x, y));
			}
			delete testmove;
		}
	}
	Move * opponent_bestmove = NULL;
	int opponent_bestmove_score = -9999;
	
	// Iterate over all possible opponent moves, finding the one most in their favor.
	for(unsigned int i = 0; i < OpponentMoves.size(); i++)
	{
		Board * tempboard = gameboard->copy();
		tempboard->doMove(OpponentMoves[i], otherside);
		int pd = copiedboard->count(otherside) - copiedboard->count(side);
		if (pd > opponent_bestmove_score)
		{
			opponent_bestmove = new Move((OpponentMoves[i])->x, (OpponentMoves[i])->y);
		}
		delete tempboard;
		delete OpponentMoves[i];
		
	}
	
	// Perform the opponent's best move.
	copiedboard->doMove(opponent_bestmove, otherside);
	
	// Calculate piece difference after opponent's move (this will be the move's score).
	int piece_difference = copiedboard->count(side) - copiedboard->count(otherside);
	delete copiedboard;
	this->score = (piece_difference);
}

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

	// Declare a board object to keep track of the board and a move object
	// to track the opponent's last move.
	this->gameboard = new Board();
	this->opponent_move = NULL;
	this->side = side;
	if (this->side == WHITE)
	{
		this->opponent_side = BLACK;
	}
	else
	{
		this->opponent_side = WHITE;
	}
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete this->gameboard;
	delete this->opponent_move;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
 
 
 
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
	 * Must return a move or null if none are possible.
     */ 
	// Enter opponent's move on AI board.
	this->opponent_move = opponentsMove;
	this->gameboard->doMove(opponentsMove, this->opponent_side);
	
	// If you have no available moves, return null immediately.
	if(not this->gameboard->hasMoves(this->side))
	{
		return NULL;
	}
	
	// Calculate possible moves, and put them into the PossibleMoves vector
	this->PossibleMoves = new std::vector<ScoredMove*>();
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Move * testmove = new Move(x, y);
			if(this->gameboard->checkMove(testmove, this->side))
			{
				// If move is valid, append to possible moves vector.
				this->PossibleMoves->push_back(new ScoredMove(testmove, this->side, gameboard));
				//std::cerr << "Move: x: " << testmove->x << " y: " << testmove->y << std::endl;
			}
			delete testmove;
		}
	}
	// Once possible moves have been determined, identify and make the highest scoring one.
	ScoredMove * bestmove = (*(PossibleMoves))[0];
	for(unsigned int i = 1; i < PossibleMoves->size(); i++)
	{
		if ((*(PossibleMoves))[i]->score > bestmove->score)
		{
			bestmove = (*(PossibleMoves))[i];
		}
	}
	this->gameboard->doMove(bestmove->m, this->side);
	return bestmove->m;
}