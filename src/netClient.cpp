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

NetClient::NetClient(sockaddr_in host)
{
	sock = -1;
	this->host = host;
}

NetClient::~NetClient()
{
	if(sock != -1)
	{
		shutdown(sock, SHUT_RDWR);
		close(sock);
	}
}

bool NetClient::poll_reply(Move &move)
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

		int n = read(sock, &move, sizeof(Move));

		if(n != sizeof(Move))
		{
			fprintf(stderr, "Error receiving move\n");
			exit(1);
		}

		print_move("received: ", move);

		shutdown(sock, SHUT_RDWR);
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

	return true;
}

void NetClient::receive_game_dimensions(int &width, int &height, int &connect_len, int &nb_connect)
{
	int infos[4] = {0};

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

	int n = read(sock, infos, sizeof(infos));

	if(n != sizeof(infos))
	{
		fprintf(stderr, "Error receiving infos\n");
		exit(4);
	}

	width = infos[0];
	height = infos[1];
	connect_len = infos[2];
	nb_connect = infos[3];

	shutdown(sock, SHUT_RDWR);
	close(sock);
	sock = -1;
}
