#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class TextInput
{
public:
	TextInput(int width, int height, unsigned int limit, char* font_path,
			int x = 0, int y = 0);
	~TextInput();

	void set_xy(int, int);
	int get_x();
	int get_y();

	void capture_text(SDL_Surface*);
	void display(SDL_Surface*);
	std::string get_text();
	void set_text(std::string);
	void reset();

private:
	std::string text;
	int limit;
	SDL_Surface *surface;
	SDL_Surface *background;
	TTF_Font *font;
	SDL_Color color;
	SDL_Rect position;
};

#endif
