#ifndef NETHOST_HPP
#define NETHOST_HPP

#include <sys/socket.h>
#include "net.hpp"

class NetHost : public Net
{
public:
	NetHost(uint16_t);

	virtual bool poll_reply(Move&);
	virtual bool send_move(const Move&);

private:
	int responseSocket;
};

#endif
