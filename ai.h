#ifndef AI_H
#define AI_H

#include "game.h"
#include "config.h"

class AI
{
public:
	AI(char, int);
	
	int evaluate(Game&);
	void eval_lines(Game&, int&);
	void eval_cols(Game&, int&);
	void eval_diagonals(Game&, int&);
	
	bool possible_connect_line(Game&, int, int, int);
	bool possible_connect_col(Game&, int, int, int);
	inline int calc_score(int, int);

	void make_move(Game&);
	int max(Game&);
	int min(Game&);

private:
	char player_id;
	int depth;
};

#endif
