#include <SDL/SDL.h>
#include "game.h"

class Gui
{
public:
	Gui(int, int);
	~Gui();

	void play(Game&);
	void play_vs_ai(Game&, int);
	inline void alternate_player(char&);
		
	static int ask_value(SDL_Surface*, char*, int def = -1);
	static void ask_game_dimensions(SDL_Surface*, int&, int&, int&, int&);
	static int ask_difficulty(SDL_Surface*);
	void winner(Game&);

private:
	void display_board(Game&);

	int size;	
	SDL_Surface *screen;
	SDL_Surface **sprites;
};
