#include "client.h"
#include "wsconn.h"
discordClient::discordClient(std::string token) {
	std::cout << "Enter client" << std::endl;
	discord_token = token;
	run_thread = NULL;
}

discordClient::~discordClient(void) {
	if(webSocket != NULL) {
		delete webSocket;
	}
	if(run_thread != NULL) {
		delete run_thread;
	}
}

void discordClient::connect() {
	std::cout << "Attempting connection over websocket" << std::endl;
	if(webSocket == NULL) {
		webSocket = new discordWebSocket(discord_token);
		run_thread = new boost::thread(discordWebSocket::run, (void *) webSocket);
		std::cout << "Connection thread started" << std::endl;
	} else std::cout << "A websocket already exists!" << std::endl;
}
