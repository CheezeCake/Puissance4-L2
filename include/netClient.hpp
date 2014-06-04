#ifndef NETCLIENT_HPP
#define NETCLIENT_HPP

#include <sys/socket.h>
#include "net.hpp"

class NetClient : public Net
{
public:
	NetClient(sockaddr_in);

	virtual bool poll_reply(Move&);
	virtual bool send_move(const Move&);

private:
	sockaddr_in host;
};

#endif
