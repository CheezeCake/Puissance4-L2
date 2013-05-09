#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include "game.h"
#include "gui.h"
#include "button.h"
#include "textInput.h"
#include "config.h"

using namespace std;

SDL_Surface* setup_new_screen();

int main(int argc, char **argv)
{
	//eviter warning unused (argc, argv necessaires pour SDL)
	(void)argc;
	(void)argv;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_EnableUNICODE(1);

	SDL_Surface *screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_HWSURFACE);
	bool done = false;
	SDL_Event event;
	Button one((char*)"images/button_1vs1.png");
	one.set_xy(WIDTH/3, HEIGHT/10);
	

	while(!done)
	{
		SDL_WaitEvent(&event);

		if(event.type == SDL_MOUSEBUTTONUP)
		{
			if(one.is_clicked(event.button.x, event.button.y))
			{
				int width, height, connect_len, nb_connect;
				Gui::ask_value(screen, (char*)"Largeur", width, 7);
				Gui::ask_value(screen, (char*)"Hauteur", height, 6);
				Gui::ask_value(screen, (char*)"Taille alignements", connect_len, 4);
				Gui::ask_value(screen, (char*)"Nombre d'alignements", nb_connect, 1);

				Game game(width, height, connect_len, nb_connect);
				Gui gui(width, height);
				
				gui.play(game);
				gui.winner(game);
				
				screen = setup_new_screen();
			}
		}
		
		SDL_PollEvent(&event);	
		if(event.type == SDL_QUIT)
			done = true;
		

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		one.display(screen);

		SDL_Flip(screen);
	}
	
	SDL_Quit();
	TTF_Quit();
					
	return 0;
}

SDL_Surface* setup_new_screen()
{
	//libere tous les "ecrans" alloués avec SDL_SetVideoMode
	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	//redemarrage du système video de SDL
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_EnableUNICODE(1);

	//création d'un nouvel "ecran"
	return SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_HWSURFACE);
}
