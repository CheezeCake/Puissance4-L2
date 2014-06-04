#include <iostream>
#include "textInput.hpp"

using namespace std;

TextInput::TextInput(int width, int height, unsigned int limit, char* font_path,
		int x, int y)
{
	this->limit = limit;
	position.x = x;
	position.y = y;

	surface = NULL;
	background = NULL;
	background = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32,
			0, 0, 0, 0);
	if(background == NULL)
	{
		cerr << "TextInput: Erreur lors de la creation d'une SDL_Surface ("
			<< SDL_GetError() << ")\n";
		exit(1);
	}

	color.r = color.g = color.b = 0;

	font = NULL;
	font = TTF_OpenFont(font_path, (4*height)/5);
	if(font == NULL)
	{
		cerr << "TextInput: Erreur lors du chargement de la police ("
			<< font_path << ")\n";
		exit(1);
	}
}

TextInput::~TextInput()
{
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}

void TextInput::set_xy(int x, int y)
{
	position.x = x;
	position.y = y;
}

int TextInput::get_x()
{
	return position.x;
}

int TextInput::get_y()
{
	return position.y;
}

void TextInput::capture_text(SDL_Surface *screen)
{
	SDL_Event event;
	bool done = false;

	display(screen);

	while(!done)
	{
		SDL_WaitEvent(&event);

		if(event.type == SDL_QUIT)
		{
			//remettre l'evenement dans la file
			SDL_PushEvent(&event);
			return;
		}
		else if(event.type == SDL_KEYDOWN)
		{
			if(event.key.keysym.sym == SDLK_RETURN)
				done = true;
			else if((event.key.keysym.sym == SDLK_BACKSPACE) && (text.size() > 0))
				text.erase(text.end()-1);
			else if((event.key.keysym.unicode >= 33) &&
					(event.key.keysym.unicode <= 126) && ((int)text.size() <= limit))
				text.push_back((char)event.key.keysym.unicode);
		}

		display(screen);
	}
}

void TextInput::display(SDL_Surface *screen)
{
	SDL_FreeSurface(surface);
	surface = NULL;
	if(!text.empty())
		surface = TTF_RenderText_Blended(font, text.c_str(), color);

	SDL_FillRect(background, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
	SDL_BlitSurface(background, NULL, screen, &position);

	if(surface != NULL)
	{
		SDL_Rect _position;
		_position.x = position.x+background->h/10;
		_position.y = position.y+background->h/10;
		SDL_BlitSurface(surface, NULL, screen, &_position);
	}

	SDL_Flip(screen);
}

string TextInput::get_text()
{
	return text;
}

void TextInput::set_text(string s)
{
	text = s;
}

void TextInput::reset()
{
	text = "";
}
