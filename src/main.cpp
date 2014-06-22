#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "game.hpp"
#include "gui.hpp"
#include "button.hpp"
#include "textInput.hpp"
#include "config.hpp"
#include "netHost.hpp"
#include "netClient.hpp"

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
	Button vs_ai((char*)"images/button_vs_ia.png");
	vs_ai.set_xy(WIDTH/3, HEIGHT/3);
	Button net_host((char*)"images/button_network_host.png");
	net_host.set_xy(WIDTH/6, HEIGHT/1.5);
	Button net_client((char*)"images/button_network_client.png");
	net_client.set_xy(WIDTH/1.7, HEIGHT/1.5);
	

	while(!done)
	{
		SDL_WaitEvent(&event);

		if(event.type == SDL_MOUSEBUTTONUP)
		{
			int width, height, connect_len, nb_connect;
			if(one.is_clicked(event.button.x, event.button.y))
			{
				Gui::ask_game_dimensions(screen, width, height, connect_len, nb_connect);

				Game game(width, height, connect_len, nb_connect);
				Gui gui(width, height);
				
				gui.play(game);
				gui.winner(game);
				
				screen = setup_new_screen();
			}
			else if(vs_ai.is_clicked(event.button.x, event.button.y))
			{
				Gui::ask_game_dimensions(screen, width, height, connect_len, nb_connect);
				int difficulty = Gui::ask_difficulty(screen);
				
				if(difficulty != -1)
				{
					Game game(width, height, connect_len, nb_connect);
					Gui gui(width, height);

					gui.play_vs_ai(game, difficulty);
					gui.winner(game);
					screen = setup_new_screen();
				}
				else
					done = true;
			}
			else if(net_host.is_clicked(event.button.x, event.button.y))
			{
				uint16_t listen_port;

				Gui::ask_game_dimensions(screen, width, height, connect_len, nb_connect);
				listen_port = Gui::ask_value(screen, (char*)"Listenning port", 6666);

				Game game(width, height, connect_len, nb_connect);
				Gui gui(width, height);

				SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
				SDL_Flip(screen);
				std::cout << "Waiting for incoming connection..." << '\n';

				NetHost host(listen_port);
				while(!host.send_game_dimensions(width, height, connect_len, nb_connect)
						&& !done)
				{
					SDL_PollEvent(&event);
					if(event.type == SDL_QUIT)
						done = true;
				}

				if(!done)
				{
					gui.play_net(game, host, Game::other_player(game.get_current_player()));
					gui.winner(game);

					screen = setup_new_screen();
				}
			}
			else if(net_client.is_clicked(event.button.x, event.button.y))
			{
				sockaddr_in host_in;
				memset(&host_in, 0, sizeof(host_in));

				Gui::ask_host_info(screen, &host_in);
				host_in.sin_family = PF_INET;

				SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
				SDL_Flip(screen);
				std::cout << "Connecting to host..." << '\n';

				NetClient client(host_in);
				while(!client.receive_game_dimensions(width, height, connect_len, nb_connect)
						&& !done)
				{
					SDL_PollEvent(&event);
					if(event.type == SDL_QUIT)
						done = true;
				}

				if(!done)
				{
					Game game(width, height, connect_len, nb_connect);
					Gui gui(width, height);

					gui.play_net(game, client, game.get_current_player());
					gui.winner(game);

					screen = setup_new_screen();
				}
			}
		}
		
		SDL_PollEvent(&event);	
		if(event.type == SDL_QUIT)
			done = true;
		

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		one.display(screen);
		vs_ai.display(screen);
		net_host.display(screen);
		net_client.display(screen);

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
