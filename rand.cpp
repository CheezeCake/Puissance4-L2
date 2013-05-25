#include <cstdlib>
#include <ctime>
#include "rand.h"

unsigned int Rand::count = 0;

void Rand::check_seed()
{
	if(count == 0 || count > MAX_GEN)
	{
		srand(time(NULL));
		count = 0;
	}
}

bool Rand::rand()
{
	check_seed();
	++count;
	return (rand() < 0.5);
}
