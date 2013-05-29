#include <iostream>
#include <cstdlib>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "gui.hpp"
#include "button.hpp"
#include "textInput.hpp"
#include "ai.hpp"
#include "config.hpp"

using namespace std;

Gui::Gui(int width, int height)
{
	size = (width > height) ? width : height;

	screen = SDL_SetVideoMode(size*SPRITE_WIDTH, size*SPRITE_HEIGHT,
	                          BPP, SDL_HWSURFACE);
		
	if(screen == NULL)
	{
		cerr << "Erreur initialisation affichage (" 
			 << SDL_GetError() << ")" << endl;
		exit(1);
	}

	//chargement images
	sprites = new SDL_Surface*[3];
	for(int i = 0; i < 3; i++)
		sprites[i]  = NULL;
	
	sprites[PLAYER_1] = IMG_Load("images/player1.png");
	sprites[PLAYER_2] = IMG_Load("images/player2.png");
	sprites[EMPTY] = IMG_Load("images/empty.png");

	for(int i = 0; i < 3; i++)
	{
		if(sprites[i] == NULL)
		{
			cerr << "Erreur lors du chargement d'une sprite" << endl;
			exit(1);
		}
	}
}

Gui::~Gui()
{
	for(int i = 0; i < 3; i++)
		SDL_FreeSurface(sprites[i]);
}

void Gui::display_board(Game &g)
{
	int diff_x = size-g.get_width();
	int diff_y = size-g.get_height();

	SDL_Rect pos;
	for(int i = 0; i < g.get_height(); i++)
	{
		for(int j = 0; j < g.get_width(); j++)
		{
			pos.x = (j+diff_x/2)*SPRITE_WIDTH;
			pos.y = (i+diff_y/2)*SPRITE_HEIGHT;

			SDL_BlitSurface(sprites[(int)g.get_value(i, j)], NULL, screen, &pos);
		}
	}
}

void Gui::play(Game &g)
{
	bool done = false;
	SDL_Event event;

	while(!done)
	{
		SDL_WaitEvent(&event);
	
		if(event.type == SDL_QUIT)
		{
			done = true;
			//on replace l'evènement dans la file
			SDL_PushEvent(&event);
		}
		else if(event.type == SDL_MOUSEBUTTONUP)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			   g.make_move((event.button.x/SPRITE_WIDTH)-(size-g.get_width())/2);
		}
		else if(event.type == SDL_KEYUP)
		{
			int way = -1;
			if(event.key.keysym.sym == SDLK_RIGHT)
				way = RIGHT;
			else if(event.key.keysym.sym == SDLK_LEFT)
				way = LEFT;

			if(way != -1)
				g.rotate(way);
		}
		
		if(g.done())
			done = true;
			
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		display_board(g);

		SDL_Flip(screen);
	}
}

void Gui::play_vs_ai(Game &g, int difficulty)
{
	bool done = false;
	SDL_Event event;

	char player = g.get_current_player();
	char player_ai = Game::other_player(player);
	AI ai(&g, player_ai, difficulty);
	
	while(!done)
	{
		if(g.get_current_player() == player_ai)
			ai.make_move();
		else
		{
			SDL_WaitEvent(&event);
		
			if(event.type == SDL_QUIT)
			{
				done = true;
				SDL_PushEvent(&event);
			}
			else if(event.type == SDL_MOUSEBUTTONUP)
			{
				if(event.button.button == SDL_BUTTON_LEFT)
					g.make_move((event.button.x/SPRITE_WIDTH)-(size-g.get_width())/2);
			}
			else if(event.type == SDL_KEYUP)
			{
				int way = -1;
				if(event.key.keysym.sym == SDLK_RIGHT)
					way = RIGHT;
				else if(event.key.keysym.sym == SDLK_LEFT)
					way = LEFT;

				if(way != -1)
					g.rotate(way);
			}
		}

		if(g.done())
			done = true;

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		display_board(g);
		SDL_Flip(screen);
	}
}

int Gui::ask_difficulty(SDL_Surface *screen)
{
	TTF_Font *font = TTF_OpenFont((char*)"fonts/arial.ttf", HEIGHT/20);
	SDL_Color color = {255, 255, 255, 0};
	SDL_Surface *tile = TTF_RenderText_Blended(font, (char*)"difficulte",
											   color);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
	Button easy((char*)"images/button_easy.png");
	easy.set_xy(WIDTH/2-easy.get_width()/2, HEIGHT/7);
	Button normal((char*)"images/button_normal.png");
	normal.set_xy(easy.get_x(), easy.get_y()+(easy.get_height()*1.5));
	Button hard((char*)"images/button_hard.png");
	hard.set_xy(easy.get_x(), normal.get_y()+(normal.get_height()*1.5));
	
	bool done = false;
	SDL_Event event;
	SDL_Rect pos;
	pos.x = WIDTH/2-tile->w/2;
	pos.y = HEIGHT/20;
	SDL_BlitSurface(tile, NULL, screen, &pos);
	
	while(!done)
	{
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT)
			done = true;
		else if(event.type == SDL_MOUSEBUTTONUP)
		{
			int x = event.button.x;
			int y = event.button.y;

			if(easy.is_clicked(x, y))
				return EASY;
			else if(normal.is_clicked(x, y))
				return NORMAL;
			else if(hard.is_clicked(x, y))
				return HARD;
		}

		easy.display(screen);
		normal.display(screen);
		hard.display(screen);

		SDL_Flip(screen);
	}
	return -1;
}

int Gui::ask_value(SDL_Surface *screen, char *name, int def)
{
	TTF_Font *font = TTF_OpenFont((char*)"fonts/arial.ttf", HEIGHT/20);
	SDL_Color color = {255, 255, 255, 0};
	SDL_Surface *title = TTF_RenderText_Blended(font, name, color);

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	TextInput input(WIDTH/2, HEIGHT/20, 4, (char*)"fonts/arial.ttf",
					WIDTH/4, HEIGHT/2-HEIGHT/10);

	SDL_Event event;	
	SDL_Rect pos;
	pos.x = WIDTH/2-title->w/2;
	pos.y = HEIGHT/20;
	SDL_BlitSurface(title, NULL, screen, &pos);
	
	if(def != -1)
	{
		ostringstream oss;
		oss << def;
		input.set_text(oss.str());
	}
	
	int value;	
	do
	{	
		input.capture_text(screen);
		value = atoi(input.get_text().c_str());
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			exit(0);
		input.reset();
	} while(value <= 0);
	
	SDL_FreeSurface(title);
	TTF_CloseFont(font);

	return value;
}

void Gui::ask_game_dimensions(SDL_Surface *screen, int &width, int &height, int &connect_len,
							  int &nb_connect)
{
	width = ask_value(screen, (char*)"Largeur", 7);
	height = ask_value(screen, (char*)"Hauteur", 6);
	connect_len = ask_value(screen, (char*)"Taille alignements", 4);
	nb_connect = ask_value(screen, (char*)"Nombre d'alignements", 1);
}

void Gui::winner(Game &game)
{
	string winner = game.get_winner();
	
	if(winner.empty())
		return;
	
	SDL_Event event;
	SDL_Rect pos;
	SDL_Surface *surface = NULL;
	TTF_Font *font = TTF_OpenFont((char*)"fonts/arial.ttf", screen->h/20);
	SDL_Color color = {255, 0, 255, 0};
	
	if(!game.tie())
		winner += " a gagne !";

	surface = TTF_RenderText_Blended(font, winner.c_str(), color);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	display_board(game);
	
	pos.x = screen->w/2-surface->w/2;
	pos.y = screen->h/2-surface->h/2;
	SDL_BlitSurface(surface, NULL, screen, &pos);
	
	SDL_Flip(screen);
		
	do
	{
		SDL_WaitEvent(&event);
	
		if(event.type == SDL_QUIT)
		{
			SDL_PushEvent(&event);
			return;
		}
	} while(event.type != SDL_KEYDOWN);
}
