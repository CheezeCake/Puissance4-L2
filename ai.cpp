#include <iostream>
#include "ai.h"

using namespace std;

AI::AI(char ai_id, int difficulty)
{
	this->difficulty = difficulty;
	this->ai_id = ai_id;
	player_id = Game::other_player(ai_id);
}

void AI::make_move(Game &game)
{
	int score_max = SCORE_MIN;
	int col_max = 0;

	for(int i = 0; i < game.get_width(); i++)
	{
		if(game.make_move(i))
		{
			int score = min(game, difficulty-1);
			//cout << score << endl;
			if(score > score_max) //ajouter de l'aléatoire
			{
				score_max = score;
				col_max = i;
			}
			game.cancel_move(i);
		}
	}

	game.make_move(col_max);
}

int AI::max(Game &game, int depth)
{
	if((depth == 0) || game.done())
		return evaluate(game, depth);
	
	int score_max = SCORE_MIN;
	for(int i = 0; i < game.get_width(); i++)
	{
		if(game.make_move(i))
		{
			int score = min(game, depth-1);

			if(score > score_max)
				score_max = score;

			game.cancel_move(i);
		}
	}
	
	return score_max;
}

int AI::min(Game &game, int depth)
{
	if((depth == 0) || game.done())
		return evaluate(game, depth);
	
	int score_min = SCORE_MAX;

	for(int i = 0; i < game.get_width(); i++)
	{
		if(game.make_move(i))
		{
			int score = max(game, depth-1);

			if(score < score_min)
				score_min = score;
			
			game.cancel_move(i);
		}
	}

	return score_min;
}

/*
	TODO: verif si connect impossible
*/

int AI::evaluate(Game &game, int depth)
{
	if(game.done())
	{
		if(game.tie())
			return SCORE_TIE;

		return (game.get_current_player() == game.other_player(ai_id))
				? SCORE_MAX+depth+1 : SCORE_MIN-depth-1;
	}

	int score = 0;
	score += game.get_nb_connect(ai_id)*SCORE_CONNECT;
	score -= game.get_nb_connect(player_id)*SCORE_CONNECT;

	eval_lines(game, score);

	return score;
}

int AI::score_possible_connect(Game &game, int i, int j, int size)
{
	int connect_len = game.get_connect_len();
	if(game.get_width()-size < connect_len-size)
		return 0;

	int spaces_around = 0;
	int count = 0;
	for(int k = 0; k < j-size; k++)
	{
		if(game.get_value(i, k) == EMPTY)
			++count;
		else
			count = 0;
	}

	spaces_around += count;
	count = 0;
	for(int k = j; (game.get_value(i, k) == EMPTY) && (k < game.get_width()); k++)
		++count;

	spaces_around += count;

	if(size+spaces_around < connect_len)
		return 0;

	int score = SCORE_CONNECT/(connect_len-size);
	return (game.get_current_player() == ai_id) ? score : -score;
}

void AI::eval_lines(Game &game, int &score)
{
	for(int i = 0; i < game.get_height(); i++)
	{
		int count = (game.get_value(i, 0) == EMPTY) ? 0 : 1;
		for(int j = 1; j < game.get_width(); j++)
		{
			char current = game.get_value(i, j);

			if(current == game.get_value(i, j-1) && current != EMPTY)
				++count;
			else if(count > 0)
			{
				score += score_possible_connect(game, i, j, count);
				count = 0;
			}
		}
	}
}
