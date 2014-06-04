#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include "netHost.hpp"
#include "game.hpp"

NetHost::NetHost(uint16_t port)
{
	sockaddr_in addr;

	sock = -1;
	responseSocket = -1;

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		perror("Socket");
		return;
	}

	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;


	if(bind(sock, (sockaddr*) &addr, sizeof(addr)) == -1)
	{
		perror("Bind");
		return;
	}

	if(listen(sock, 1) == -1)
		perror("Listen");

	std::cout << "Listening on port " << port << '\n';
}

bool NetHost::poll_reply(Move &move)
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
		if((responseSocket = accept(sock, NULL, 0)) == -1)
		{
			perror("Accept");
			return false; //exit ?
		}

		printf("... ");

		int n = read(responseSocket, &move, sizeof(Move));

		if(n != sizeof(Move))
		{
			fprintf(stderr, "Error receiving move\n");
			exit(1);
		}

		print_move("received: ", move);
	}

	return true;
}

bool NetHost::send_move(const Move &move)
{
	if(responseSocket == -1)
		exit(2);

	int n = write(responseSocket, &move, sizeof(Move));

	if(n != sizeof(Move))
	{
		fprintf(stderr, "Error sending move\n");
		exit(3);
	}

	print_move("send: ", move);

	close(responseSocket);
	responseSocket = -1;

	return true;
}
