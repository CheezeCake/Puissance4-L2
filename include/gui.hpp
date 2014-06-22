#include <SDL/SDL.h>
#include <sys/socket.h>
#include "game.hpp"
#include "net.hpp"

class Gui
{
public:
	Gui(int, int);
	~Gui();

	void play(Game&);
	void play_vs_ai(Game&, int);
	void play_net(Game&, Net&, char);

	static int ask_value(SDL_Surface*, char*, int def = -1);
	static void ask_game_dimensions(SDL_Surface*, int&, int&, int&, int&);
	static int ask_difficulty(SDL_Surface*);
	static void ask_host_info(SDL_Surface*, sockaddr_in*);
	static void ask_ip_address(SDL_Surface*, char*, sockaddr_in*);
	void winner(Game&);

private:
	void display_board(Game&);

	int size;
	SDL_Surface *screen;
	SDL_Surface **sprites;
};
