#include <string>
#include <iostream>
#include <wsconn.h>
#ifndef CLIENT_H
#define CLIENT_H

class discordClient {
public:
	discordClient(std::string token);
	void connect();
private:
	std::string discord_token;
	discordWebSocket *webSocket = NULL;
};
#endif