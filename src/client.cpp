#include <client.h>
#include <wsconn.h>
discordClient::discordClient(std::string token) {
	std::cout << "Enter client" << std::endl;
	discord_token = token;
}

void discordClient::connect() {
	std::cout << "Attempting connection over websocket" << std::endl;
	if(webSocket == NULL) {
		webSocket = new discordWebSocket(discord_token);
    	pthread_t pt_r;
    	pthread_create(&pt_r, NULL, discordWebSocket::run, (void *) webSocket);
		std::cout << "Connection thread started" << std::endl;
	} else std::cout << "A websocket already exists!" << std::endl;
}