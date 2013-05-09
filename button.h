#include <SDL/SDL.h>

class Button
{
public:
	Button(char*);
	~Button();

	void display(SDL_Surface*);
	void set_x(int);
	void set_y(int);
	void set_xy(int, int);
	bool is_clicked(int, int);

private:
	int x;
	int y;
	SDL_Surface *surface;
};
