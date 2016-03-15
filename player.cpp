#include "player.h"

std::vector<Move *> * PossibleMoves(Board * gameboard, Side side)
{
	std::vector<Move *> * PossibleMoves = new std::vector<Move *>();
	for(int x = 0; x < 8; x++){
		for(int y = 0; y < 8; y++){
			Move * testmove = new Move(x, y);
			if (gameboard->checkMove(testmove, side)){
				PossibleMoves->push_back(new Move(x, y));
			}
			delete testmove;
			}
		}
	return PossibleMoves;
	}


/* Mingain function:
 * This function takes a move, a board, and the sides as arguments.
 * It then returns the minimum gain of that move for the side passed as an argument by
 * finding the worst possible piece count outcome after one opponent move.*/
 
int mingain(Move * move, Board * board, Side side, Side otherside)
{
	// Generates pointer to vector of pointers to possible opponent moves
	std::vector<Move *> * opponent_moves = PossibleMoves(board, otherside);
	// Initial piece difference (before any moves made, including argument move)
	int init_piece_diff = board->count(side) - board->count(otherside);
	int worstgain = 9999;
	// Copied board to modify without affecting game state.
	Board * copiedboard = board->copy();
	copiedboard->doMove(move, side);
	for(unsigned int i = 0; i < opponent_moves->size(); i ++)
	{
		Board * gameboard = copiedboard->copy();
		gameboard->doMove(((*opponent_moves)[i]), otherside);
		int piece_diff = gameboard->count(side) - gameboard->count(otherside);
		if (piece_diff - init_piece_diff < worstgain)
		{
			worstgain = piece_diff - init_piece_diff;
		}
		delete gameboard;
	}
	delete copiedboard;
	delete opponent_moves;
	return worstgain;
}

// Constructor for ScoredMove type
ScoredMove::ScoredMove(Move * move, int score)
{
	/* I initialize m in this convulted way so that the pointer it is given
	 * can be safely freed in the program to save memory.
	 */
	this->m = new Move(move->x, move->y);
	this->score = score;
	
}

ScoredMove::~ScoredMove(){
	delete this->m;
}

Move* bestMove(std::vector<Move *>* PossibleMoves, Board * gameboard, Side side, Side otherside)
{
	/* This function takes a vector of possible moves and returns the lowest scoring one.
	 * This function should
	 */
	 ScoredMove* best = new ScoredMove((*PossibleMoves)[0], -9999);
	 // Score is simply end difference in number of pieces after another turn by opponent.a
	 for(unsigned int i = 0; i < PossibleMoves->size(); i++)
	 {
		 // SCORE CALCULATION FOR A MOVE BEGINS HERE
		 float score;
		 // Coin difference begins here
		 Board * testboard = gameboard->copy();
		 testboard->doMove((*PossibleMoves)[i], side);
		 float coindiff = (testboard->count(side) - testboard->count(otherside)) / (testboard->count(side)+testboard->count(otherside));
		 coindiff *= 2500;
		 
		 score = coindiff;
		 // Coin difference ends here
		 
		 // Mobility score evaluation begins here
		 int yourmoves = testboard->countMoves(side);
		 int theirmoves = testboard->countMoves(otherside);
		 if(yourmoves != 0 && theirmoves != 0)
		 {
			 float mobScore = (yourmoves - theirmoves) / (yourmoves + theirmoves);
			 mobScore *= 500;
			 score += mobScore;
		 }
		 // Mobility score evaluation ends here
		 
		 // Corners captured count starts here
		 float wcorners = 0;
		 float bcorners = 0;
		 if(testboard->coins[0][0] == 1)
		 {
			 bcorners ++;
		 }
		 else if(testboard->coins[0][0] == 2)
		 {
			 wcorners ++;
		 }
		 if(testboard->coins[0][7] == 1)
		 {
			 bcorners ++;
		 }
		 else if(testboard->coins[0][7] == 2)
		 {
			 wcorners ++;
		 }
		 if(testboard->coins[7][0] == 1)
		 {
			 bcorners ++;
		 }
		 else if(testboard->coins[7][0] == 2)
		 {
			 wcorners ++;
		 }
		 if(testboard->coins[7][7] == 1)
		 {
			 bcorners ++;
		 }
		 else if(testboard->coins[7][7] == 2)
		 {
			 wcorners ++;
		 }
		 if (side == BLACK)
		 {
			float cornerscore = (bcorners - wcorners) / (bcorners + wcorners);
			 cornerscore *= 3000;
			 score += cornerscore;
		 }
		 else
		{
			 float cornerscore = (wcorners - bcorners) / (bcorners + wcorners);
			 cornerscore *= 3000;
			 score += cornerscore;
		 }
		 // Corners captured count ends here
		 
		 // Stability count starts here
		 
		 
		 // Stability count ends here
		 
		 // SCORE CALCULATION FOR A MOVE ENDS HERE
		 delete testboard;
		 if (score > best->score)
		 {
			 best = new ScoredMove(new Move((*PossibleMoves)[i]->x, (*PossibleMoves)[i]->y), score);
		 }
	 }
	 delete PossibleMoves;
	 return best->m;
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
	
	//If available moves, return the highest-scoring one.
	Move * play = bestMove(PossibleMoves(this->gameboard, this->side), this->gameboard, this->side, this->opponent_side);
	this->gameboard->doMove(play, this->side);
	return play;
}