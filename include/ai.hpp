#ifndef AI_H
#define AI_H

#include "game.hpp"
#include "config.hpp"

const int SCORE_MAX = 1000000;
const int SCORE_MIN = -1000000;
const int SCORE_TIE = 0;
const int SCORE_CONNECT = 300;

class AI
{
public:
	AI(Game*, char, int);
	~AI();

	void make_move();

private:
	int evaluate(int);
	void eval_lines(int&);
	void eval_cols(int&);

	inline char get_value(int, int, bool);
	int score_possible_connect(int, int, int, bool);

	int min(int, int);
	int max(int, int);
	inline bool change_score_max(int&, int);
	inline bool change_score_min(int&, int);

	Game *game;
	char **save;
	int dim;

	char ai_id;
	char player_id;
	int difficulty;
};

#endif
