#include <iostream>
#include <algorithm>
#include "ai.hpp"

using namespace std;

AI::AI(Game *game, char ai_id, int difficulty)
{
	this->game = game;
	this->difficulty = difficulty;
	this->ai_id = ai_id;
	player_id = Game::other_player(ai_id);
}

void AI::make_move()
{
	int score_max = SCORE_MIN-difficulty;
	int col_max = 0;
	bool rotate = false;
	int direction = LEFT;

	for(int i = 0; i < game->get_width(); i++)
	{
		if(game->make_move(i))
		{
			int score = min(difficulty-1, score_max);
			game->cancel_move(i);

			if(change_score_max(score_max, score))
				col_max = i;
		}
	}

	int h = game->get_height();
	int w = game->get_width();
	char **save = Game::create_board(w, h);
	game->copy_board(save);

	int dir[2] = {LEFT, RIGHT};
	for(int i = 0; i < 2; i++)
	{
		game->rotate(dir[i]);
		int score = min(difficulty-1, score_max);
		game->load_board(save, w, h, ai_id);

		if((rotate = change_score_max(score_max, score)))
			direction = dir[i];
	}
	Game::delete_board(save, h);

	if(rotate)
		game->rotate(direction);
	else
		game->make_move(col_max);
}

int AI::min(int depth, int alpha)
{
	if((depth == 0) || game->done())
		return evaluate(depth);
	
	int score_min = SCORE_MAX+difficulty;
	for(int i = 0; i < game->get_width(); i++)
	{
		if(game->make_move(i))
		{
			int score = max(depth-1, score_min);
			game->cancel_move(i);
			
			if(score < alpha)
				return score;

			if(change_score_min(score_min, score))
				score_min = score;
		}
	}

	int h = game->get_height();
	int w = game->get_width();
	char **save = Game::create_board(w, h);
	game->copy_board(save);

	int dir[2] = {LEFT, RIGHT};
	for(int i = 0; i < 2; i++)
	{
		game->rotate(dir[i]);
		int score = max(depth-1, score_min);
		game->load_board(save, w, h, player_id);
		
		if(score < alpha)
			return alpha;

		if(change_score_min(score_min, score))
			score_min = score;
	}
	Game::delete_board(save, h);

	return score_min;
}

int AI::max(int depth, int beta)
{
	if((depth == 0) || game->done())
		return evaluate(depth);
	
	int score_max = SCORE_MIN-difficulty;
	for(int i = 0; i < game->get_width(); i++)
	{
		if(game->make_move(i))
		{
			int score = min(depth-1, score_max);
			game->cancel_move(i);


			if(score > beta)
				return score;

			if(change_score_max(score_max, score))
				score_max = score;
		}
	}

	int h = game->get_height();
	int w = game->get_width();
	char **save = Game::create_board(w, h);
	game->copy_board(save);

	int dir[2] = {LEFT, RIGHT};
	for(int i = 0; i < 2; i++)
	{
		game->rotate(dir[i]);
		int score = min(depth-1, score_max);
		game->load_board(save, w, h, ai_id);
		
		if(score > beta)
			return score;

		if(change_score_max(score_max, score))
			score_max = score;
	}
	Game::delete_board(save, h);

	return score_max;
}

bool AI::change_score_max(int &score_max, int score)
{
	if(score > score_max)
	{
		score_max = score;
		return true;
	}

	return false;
}

bool AI::change_score_min(int &score_min, int score)
{
	if(score < score_min)
	{
		score_min = score;
		return true;
	}

	return false;
}

int AI::evaluate(int depth)
{
	if(game->done())
	{
		if(game->tie())
			return SCORE_TIE;

		return (game->get_winner_id() == ai_id) ? SCORE_MAX+depth+1
												: SCORE_MIN-depth-1;
	}

	int score = 0;
	score += game->get_nb_connect(ai_id)*SCORE_CONNECT;
	score -= game->get_nb_connect(player_id)*SCORE_CONNECT;

	eval_lines(score);
	eval_cols(score);

	return score;
}

inline char AI::get_value(int i, int k, bool line)
{
	return (line) ? game->get_value(i, k) : game->get_value(k, i);
}

int AI::score_possible_connect(int i, int j, int size, bool line)
{
	int connect_len = game->get_connect_len();
	int dim_size = (line) ? game->get_width() : game->get_height();
	if(dim_size-size < connect_len-size)
		return 0;

	int spaces_around = 0;
	int count = 0;
	for(int k = 0; k < j-size; k++)
	{
		if(get_value(i, k, line) == EMPTY)
			++count;
		else
			count = 0;
	}

	spaces_around += count;
	count = 0;
	for(int k = j; (get_value(i, k, line) == EMPTY) && (k < dim_size); k++)
		++count;

	spaces_around += count;

	if(size+spaces_around < connect_len)
		return 0;

	return size;
}

void AI::eval_lines(int &score)
{
	for(int i = 0; i < game->get_height(); i++)
	{
		int count = (game->get_value(i, 0) == EMPTY) ? 0 : 1;
		for(int j = 1; j < game->get_width(); j++)
		{
			char current = game->get_value(i, j);

			if((current == game->get_value(i, j-1)) && (current != EMPTY))
				++count;
			else if(count > 0)
			{
				int s = score_possible_connect(i, j, count, true);
				if(game->get_value(i, j-1) == player_id)
					s = -s;
				score += s;
				count = 0;
			}
		}
	}
}

void AI::eval_cols(int &score)
{
	for(int i = 0; i < game->get_width(); i++)
	{
		int count = (game->get_value(0, i) == EMPTY) ? 0 : 1;
		for(int j = 1; j < game->get_height(); j++)
		{
			char current = game->get_value(j, i);

			if((current == game->get_value(j, i-1)) && (current != EMPTY))
				++count;
			else if(count > 0)
			{
				int s = score_possible_connect(i, j, count, false);
				if(game->get_value(j-1, i) == player_id)
					s = -s;
				score += s;
				count = 0;
			}
		}
	}
}

