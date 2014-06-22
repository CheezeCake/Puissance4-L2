#ifndef NETCLIENT_HPP
#define NETCLIENT_HPP

#include <sys/socket.h>
#include "net.hpp"

class NetClient : public Net
{
public:
	NetClient(sockaddr_in);
	virtual ~NetClient();

	virtual bool poll_reply(Move&);
	virtual bool send_move(const Move&);
	bool receive_game_dimensions(int&, int&, int&, int&);

private:
	sockaddr_in host;
};

#endif
