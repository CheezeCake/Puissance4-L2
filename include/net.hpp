#ifndef NETSRV_HPP
#define NETSRV_HPP

#include <sys/socket.h>
#include <cstdio>
#include "game.hpp"

struct Move
{
	int type;
	int column;
};

enum { INSERTION, ROTATION };

class Net
{
public:
	virtual bool poll_reply(Move&) = 0;
	virtual bool send_move(const Move&) = 0;

protected:
	void print_move(const char *prefix, const Move &move)
	{
		printf("%s", prefix);

		if(move.type == ROTATION)
		{
			if(move.column == RIGHT)
				printf("right rotation");
			else
				printf("left rotation");
		}
		else
			printf("%d", move.column);

		printf("\n");
	}

	int sock;
};

#endif
