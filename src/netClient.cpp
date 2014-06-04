#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include "netClient.hpp"
#include "game.hpp"

netClient::netClien(sockaddr_in host)
{
	sock = -1;
	this->host = host;
}

bool netClien::poll_reply(Move &move)
{
	if(sock == -1)
		return false;

	fd_set read_fd;
	FD_SET(sock, &read_fd);

	if(select(sock+1, &read_fd, NULL, NULL, NULL) == -1)
	{
		perror("Select");
		return false;
	}

	if(FD_ISSET(sock, &read_fd))
	{
		printf("... ");

		int n = read(responseSocket, &move, sizeof(Move));

		if(n != sizeof(Move))
		{
			fprintf(stderr, "Error receiving move\n");
			exit(1);
		}

		print_move("received: ", move);

		close(sock);
		sock = -1;
	}

	return true;
}

bool NetClient::send_move(const Move &move)
{
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		perror("Socket");
		exit(2);
	}

	if(connect(sock, (sockaddr*)&host, sizeof(host)) == -1)
	{
		perror("Connect");
		exit(3);
	}

	int n = write(sock, &move, sizeof(Move));

	if(n != sizeof(Move))
	{
		fprintf(stderr, "Error sending move\n");
		exit(4);
	}

	print_move("sent: ", move);

	close(sock);
	sock = -1;

	return true;
}
