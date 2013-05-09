#include <SDL/SDL.h>
#include "game.h"

class Gui
{
public:
	Gui(int, int);
	~Gui();

	void play(Game&);
	void alternate_player(int&);
		
	static void ask_value(SDL_Surface*, char*, int&, int def = -1);
	void winner(Game&);

private:
	void display_board(Game&);

	int size;	
	SDL_Surface *screen;
	SDL_Surface **sprites;
};
