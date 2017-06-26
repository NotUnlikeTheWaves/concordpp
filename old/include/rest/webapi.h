#ifndef API_H
#define API_H

#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>

class HttpAPIHandler {
public:
	HttpAPIHandler(std::string discord_bot_token);
	~HttpAPIHandler();

		// API Functionality
	void CreateMessage(unsigned long long channel_id, std::string message);
    void CreateMessage(std::string channel_id, std::string message);
private:
	std::string discord_token;
	RestClient::Connection *http_req_conn;
};

#endif
