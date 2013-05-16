#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "config.h"

enum {LEFT, RIGHT};

const int EASY = 2;
const int NORMAL = 4;
const int HARD = 6;

class Game
{
public:
	Game(int, int, int = 4, int = 1);
	~Game();

	void display_cli();
	bool make_move(int);
	void cancel_move(int);
	void rotate(int);
	
	char get_value(int, int);
	char get_current_player();
	void copy_board(char**);
	void load_board(char**);

	bool done();
	
	bool tie();
	std::string get_winner();
	int get_winner_id();
	
	int get_width();
	int get_height();
	int get_connect_len();
	int get_nb_connect();
	int get_nb_connect(char);
		
	static char** create_board(int, int);
	static void delete_board(char**, int);
	static char other_player(char);

private:
	void alternate_player();
	void gravity();
	void gravity_on_col(int);
	
	void check();
	bool update_count(char, char, int&);
	bool check_lines();
	bool check_columns();	
	bool check_diagonals1();
	bool check_diagonals2();
	bool check_tie();
	
	char **board;
	int width;
	int height;
	int connect_len; //taille alignement
	int nb_connect; //nombre alignements
	
	char current_player;
	int connections[2];
	int winner;
};

#endif
