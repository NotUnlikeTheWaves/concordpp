#include "rest/webapi.h"
#include <iostream>
HttpAPIHandler::HttpAPIHandler(std::string discord_bot_token) {
	discord_token = discord_bot_token;
	RestClient::init();
	http_req_conn = new RestClient::Connection("https://discordapp.com/api");
	http_req_conn->SetTimeout(5);
	http_req_conn->FollowRedirects(true);
	http_req_conn->SetUserAgent("NirvanaBot (none, 0.1)");
	RestClient::HeaderFields headers;
	headers["Authorization"] = "Bot " + discord_bot_token;
	http_req_conn->SetHeaders(headers);
}

HttpAPIHandler::~HttpAPIHandler() {
	delete http_req_conn;
	RestClient::disable();
}

void HttpAPIHandler::CreateMessage(unsigned long long channel_id, std::string message) {
    http_req_conn->post("/channels/" + std::to_string(channel_id) + "/messages",
        "{ \"content\": \"" + message + "\"}");
}

void HttpAPIHandler::CreateMessage(std::string channel_id, std::string message) {
    http_req_conn->post("/channels/" + channel_id + "/messages",
        "{ \"content\": \"" + message + "\"}");
}
