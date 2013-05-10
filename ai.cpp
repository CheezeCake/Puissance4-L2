#include "ai.h"

AI::AI(char player_id, int difficulty)
{
	depth = difficulty;
	this->player_id = player_id;
}

void AI::make_move(Game &game)
{
	int score_max = SCORE_MIN;
	int col_max = -1;

	for(int i = 0; i < game.get_width(); i++)
	{
		if(game.make_move(player_id, i))
		{
			int score = max(game, depth-1);

			if(score > score_max) //ajouter de l'aléatoire
			{
				score_max = score;
				col_max = i;
			}
			game.cancel_move(i);
		}
	}

	game.make_move(player_id, col_max);
}

int AI::max(Game &game, int depth)
{
	if((depth == 0) || game.done())
		return evaluate(game);
	
	int score_max = SCORE_MIN;
	int col_max = -1;

	for(int i = 0; i < game.get_width(); i++)
	{
		if(game.make_move(player_id, i))
		{
			int score = min(game, depth-1);

			if(score > score_max)
			{
				score_max = score;
				col_max = i;
			}
			game.cancel_move(i);
		}
	}
	
	return score_max;
}

int AI::min(Game &game, int depth)
{
	if((depth == 0) || game.done())
		return evaluate(game);
	
	int score_min = SCORE_MAX;
	int col_min = -1;
	char player = Game::other_player(player_id);

	for(int i = 0; i < game.get_width(); i++)
	{
		if(game.make_move(player, i))
		{
			int score = max(game, depth-1);

			if(score < score_min)
			{
				score_min = score;
				col_min = i;
			}
			game.cancel_move(i);
		}
	}

	return score_min;
}
	
bool AI::possible_connect_line(Game &game, int line, int begin, int end)
{
	int count = 0;
	int i = begin-1;
	while((game.get_value(line, i) == EMPTY) && (i >= 0))
	{
		++count;
		--i;
	}

	int empty_pieces = count;
	count = 0;
	i = end+1;
	while((game.get_value(line, i) == EMPTY) && (i < game.get_width()))
	{
		++count;
		++i;
	}

	return (empty_pieces+count >= game.get_connect_len());
}

bool AI::possible_connect_col(Game &game, int col, int begin, int end)
{
	int count = 0;
	int i = begin-1;
	while((game.get_value(i, col) == EMPTY) && (i >= 0))
	{
		++count;
		--i;
	}

	int empty_pieces = count;
	count = 0;
	i = end+1;
	while((game.get_value(i, col) == EMPTY) && (i < game.get_height()))
	{
		++count;
		++i;
	}

	return (empty_pieces+count >= game.get_connect_len());
}

int AI::calc_score(int connect_len, int count)
{
	return (connect_len/(connect_len-count))*10; //bien ?
}

int AI::evaluate(Game &game)
{
	if(game.done())
	{
		int winner = game.get_winner_id();
		if(winner == TIE)
			return 0;

		return (winner == player_id) ? SCORE_MAX : SCORE_MIN;
	}

	int score = 0;
	eval_lines(game, score);
	eval_cols(game, score);
	eval_diagonals(game, score);
	
	return score;
}

void AI::eval_lines(Game &game, int &score)
{
	for(int i = 0; i < game.get_height(); i++)
	{
		int count = 0;
		for(int j = 0; j < game.get_width(); j++)
		{
			char val = game.get_value(i, j);
			if(val == player_id)
				++count;
			else if((val == EMPTY) && (count > 0))
			{
				if(possible_connect_line(game, i, j-count, j-1))
					score += calc_score(game.get_connect_len(), count);
				count = 0;
			}

			if(count == game.get_connect_len())
				count = 0;
		}
	}
}

void AI::eval_cols(Game &game, int &score)
{
	for(int i = 0; i < game.get_width(); i++)
	{
		int count = 0;
		for(int j = 0; j < game.get_height(); j++)
		{
			char val = game.get_value(j, i);
			if(val == EMPTY)
				++count;
			else if((val == EMPTY) && (count > 0))
			{
				if(possible_connect_col(game, i, j-count, j-1))
					score += calc_score(game.get_connect_len(), count);
				count = 0;
			}

			if(count == game.get_connect_len())
				count = 0;
		}
	}
}

void AI::eval_diagonals(Game &game, int &score) {}
