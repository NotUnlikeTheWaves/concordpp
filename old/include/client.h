#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>
#include "gateway/wsconn.h"
#include <boost/thread/thread.hpp>
#include "eventhandler.h"

class discordClient {
public:
	discordClient(std::string token);
	~discordClient();
	void connect();
    eventHandler events;
private:
	boost::thread *run_thread;
	std::string discord_token;
	discordWebSocket *webSocket = NULL;
};
#endif
