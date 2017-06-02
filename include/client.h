#include <string>
#include <iostream>
#include "wsconn.h"
#include <boost/thread/thread.hpp>
#ifndef CLIENT_H
#define CLIENT_H

class discordClient {
public:
	discordClient(std::string token);
	~discordClient();
	void connect();
private:
	boost::thread *run_thread;
	std::string discord_token;
	discordWebSocket *webSocket = NULL;
};
#endif
