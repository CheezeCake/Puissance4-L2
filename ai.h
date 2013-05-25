#ifndef AI_H
#define AI_H

#include "game.h"
#include "config.h"

const int SCORE_MAX = 1000000;
const int SCORE_MIN = -1000000;
const int SCORE_TIE = 0;
const int SCORE_CONNECT = 300;

class AI
{
public:
	AI(Game*, char, int);
	
	int evaluate(int);
	void eval_lines(int&);
	void eval_cols(int&);

	inline char get_value(int, int, bool);
	int score_possible_connect(int, int, int, bool);

	void make_move();
	int max(int);
	int min(int);

private:
	Game *game;
	char ai_id;
	char player_id;
	int difficulty;
};

#endif
