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
			int score = max(game, difficulty-1);

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

int AI::evaluate(Game &game, int depth)
{
	if(game.done())
	{
		if(game.tie())
			return SCORE_TIE;

		return (game.get_winner_id() == ai_id) ? SCORE_MAX+depth : SCORE_MIN-depth;
	}

	int score = 0;
	score += game.get_nb_connect(ai_id)*SCORE_CONNECT;
	score -= game.get_nb_connect(player_id)*SCORE_CONNECT;

	return score;
}
