#include <iostream>
#include <SDL/SDL_image.h>
#include "button.hpp"
#include "config.hpp"

Button::Button(char *path)
{
	surface = NULL;
	surface = IMG_Load(path);

	if(surface == NULL)
	{
		std::cerr << "Impossible de charger: " << path << std::endl;
		exit(1);
	}

	x = y = 0;
}

Button::~Button()
{
	SDL_FreeSurface(surface);
}

void Button::display(SDL_Surface *screen)
{
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;

	SDL_BlitSurface(surface, NULL, screen, &pos);
}

void Button::set_x(int x)
{
	this->x = x;
}

void Button::set_y(int y)
{
	this->y = y;
}

void Button::set_xy(int x, int y)
{
	this->x = x;
	this->y = y;
}

bool Button::is_clicked(int x, int y)
{
	return ((x >= this->x) && (x <= this->x+surface->w) &&
	        (y >= this->y) && (y <= this->y+surface->h));
}

int Button::get_width()
{
	return surface->w;
}

int Button::get_height()
{
	return surface->h;
}

int Button::get_x()
{
	return x;
}

int Button::get_y()
{
	return y;
}
