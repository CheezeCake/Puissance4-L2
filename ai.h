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
	AI(char, int);
	
	int evaluate(Game&, int);
	void eval_lines(Game&, int&);

	int score_possible_connect(Game&, int, int, int);

	void make_move(Game&);
	int max(Game&, int);
	int min(Game&, int);

private:
	char ai_id;
	char player_id;
	int difficulty;
};

#endif
