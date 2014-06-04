#include <iostream>
#include "game.hpp"

Game::Game(int width, int height, int connect_len, int nb_connect)
{
	this->nb_connect = nb_connect;
	this->connect_len = connect_len;
	this->width = width;
	this->height = height;
	board = create_board(width, height);
	board1 = create_board(height, width);
	current_player = PLAYER_1;
	winner = EMPTY;
	connections[PLAYER_1] = connections[PLAYER_2] = 0;
}

Game::~Game()
{
	delete_board(board, height);
	delete_board(board1, width);
}

char** Game::create_board(int width, int height)
{
	char **ret = new char*[height];
	for(int i = 0; i < height; i++)
	{
		ret[i] = new char[width];
		for(int j = 0; j < width; j++)
			ret[i][j] = EMPTY;
	}

	return ret;
}

void Game::delete_board(char **b, int h)
{
	for(int i = 0; i < h; i++)
		delete [] b[i];
	delete [] b;
}

void Game::copy_board(char **dest)
{
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			dest[i][j] = board[i][j];
}

void Game::load_board(char **src, int w, int h, char player_id)
{
	if(w == height && h == width)
	{

		current_player = player_id;
		char **tmp = board;
		board = board1;
		board1 = tmp;

		height = h;
		width = w;
		for(int i = 0; i < height; i++)
			for(int j = 0; j < width; j++)
				board[i][j] = src[i][j];
		check();
	}
}

void Game::display_cli()
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			std::cout << " ___";
		std::cout << "\n";

		std::cout << "|";
		for(int j = 0; j < width; j++)
		{
			if(board[i][j] == PLAYER_1)
				std::cout << " X ";
			else if(board[i][j] == PLAYER_2)
				std::cout << " O ";
			else
				std::cout << "   ";
			std::cout << "|";
		}
		std::cout << "\n";
	}

	for(int i = 0; i < width; i++)
		std::cout << " ___";
	std::cout << "\n";
}

char Game::get_value(int i, int j)
{
	return board[i][j];
}

char Game::other_player(char player_id)
{
	return (player_id == PLAYER_1) ? PLAYER_2 : PLAYER_1;
}

void Game::alternate_player()
{
	current_player = other_player(current_player);
}

char Game::get_current_player()
{
	return current_player;
}

bool Game::make_move(int col)
{
	if(board[0][col] != EMPTY)
		return false;

	board[0][col] = current_player;
	alternate_player();
	gravity_on_col(col);

	check(); //set winner

	return true;
}

void Game::cancel_move(int j)
{
	int i = 0;
	while((board[i][j] == EMPTY) && (i < height))
		++i;

	if(i == height)
		return;

	board[i][j] = EMPTY;

	alternate_player();
	check();
}

void Game::rotate(int direction)
{
	for(int i = 0; i < width; i++)
	{
		board1[i] = new char[height];
		for(int j = 0; j < height; j++)
		{
			if(direction == LEFT)
				board1[i][j] = board[j][width-i-1];
			else
				board1[i][j] = board[height-j-1][i];
		}
	}

	char **tmp = board;
	board = board1;
	board1 = tmp;

	int _tmp = height;
	height = width;
	width = _tmp;

	gravity();

	alternate_player();

	check();
}

inline void Game::gravity()
{
	for(int i = 0; i < width; i++)
		gravity_on_col(i);
}

void Game::gravity_on_col(int col)
{
	for(int i = height-2; i >= 0; i--)
	{
		bool fall;
		int k = i;

		do
		{
			if((board[k][col] != EMPTY) && (board[k+1][col] == EMPTY))
			{
				board[k+1][col] = board[k][col];
				board[k][col] = EMPTY;
				fall = true;
			}
			else
				fall = false;

			++k;
		} while(fall && (k < height-1));
	}
}

bool Game::tie()
{
	return (winner == TIE);
}

void Game::check_tie()
{
	//deja trouve le même nombre (gagnant) d'alignements
	//pour les deux en même temps
	if(winner == TIE)
		return;

	for(int i = 0; i < width; i++)
		if(board[0][i] == EMPTY)
			return;

	winner = TIE;
}

bool Game::done()
{
	return (winner != EMPTY);
}

void Game::check()
{
	connections[PLAYER_1] = connections[PLAYER_2] = 0;
	winner = EMPTY;

	check_lines();
	check_columns();
	check_diagonals1();
	check_diagonals2();
	check_tie();
}

void Game::update_count(char previous, char current, int &count)
{
	if(current == EMPTY)
		count = 0;
	else if(current == previous)
		++count;
	else
		count = 1;

	if(count == connect_len)
	{
		++connections[(int)current];
		count = 0;
	}

	if(connections[(int)current] == nb_connect)
	{
		if(winner == EMPTY)
			winner = current;
		else if(winner != current)
			winner = TIE;
	}

}

void Game::check_lines()
{
	if(width < connect_len)
		return;

	for(int i = height-1; i >= 0; i--)
	{
		int count = 1;
		for(int j = 1; (j < width) && (width-j >= connect_len-count); j++)
		{
			char previous = board[i][j-1];
			char current = board[i][j];

			update_count(previous, current, count);
		}
	}
}

void Game::check_columns()
{
	if(height < connect_len)
		return;

	for(int i = 0; i < width; i++)
	{
		int count = 1;
		for(int j = height-2; (j >= 0) && (connect_len-count <= j+1); j--)
		{
			char previous = board[j+1][i];
			char current = board[j][i];

			update_count(previous, current, count);
		}
	}
}

void Game::check_diagonals1()
{
	int z = std::min(width, height);
	if(z < connect_len)
		return;

	//diagonales bg -> hd
	for(int i = 0; (i < width) && (width-i >= connect_len); i++)
	{
		int start = height-1;
		int count = 1, j = 1;
		while((start-j >= 0) && (i+j < width))
		{
			char previous = board[start-j+1][i+j-1];
			char current = board[start-j][i+j];

			update_count(previous, current, count);
			++j;
		}
	}

	for(int i = height-2; (i >= 0) && (i+1 >= connect_len); i--)
	{
		int start = 0;
		int count = 1, j = 1;
		while((start+j < width) && (i-j >= 0))
		{
			char previous = board[i-j+1][start+j-1];
			char current = board[i-j][start+j];

			update_count(previous, current, count);
			++j;
		}
	}
}

void Game::check_diagonals2()
{
	int z = std::min(width, height);
	if(z < connect_len)
		return;

	//diagonales bd -> hg
	for(int i = width-1; (i >= 0) && (i+1 >= connect_len); i--)
	{
		int start = height-1;
		int count = 1, j = 1;
		while((start-j >= 0) && (i-j >= 0))
		{
			char previous = board[start-j+1][i-j+1];
			char current = board[start-j][i-j];

			update_count(previous, current, count);
			++j;
		}
	}

	for(int i = height-2; (i >= 0) && (i+1 >= connect_len); i--)
	{
		int start = width-1;
		int count = 1, j = 1;
		while((start-j >= 0) && (i-j >= 0))
		{
			char previous = board[i-j+1][start-j+1];
			char current = board[i-j][start-j];

			update_count(previous, current, count);
			++j;
		}
	}
}

std::string Game::get_winner()
{
	//partie non finie
	if(winner == EMPTY)
		return "Partie en cours";

	//ex-aequo
	if(tie())
		return "Egalite";

	return (winner == PLAYER_1) ? "Joueur 1" : "Joueur 2";
}

char Game::get_winner_id()
{
	return winner;
}

int Game::get_width()
{
	return width;
}

int Game::get_height()
{
	return height;
}

int Game::get_connect_len()
{
	return connect_len;
}

int Game::get_nb_connect()
{
	return nb_connect;
}

int Game::get_nb_connect(char player_id)
{
	return connections[(int)player_id];
}
