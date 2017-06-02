#include "api.h"
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

void HttpAPIHandler::CreateMessage(unsigned long channel_id, std::string message) {
	std::cout << "Attempt to CreateMessage: " << message << std::endl;
	http_req_conn->post("/channels/" + std::to_string(channel_id) + "/messages", 
		"{ \"content\": \"" + message + "\"}");
}


                //     // try to do HTTPS request
                // RestClient::init();
                // std::cout << "Init restclient" << std::endl;
                // RestClient::Connection *conn = new RestClient::Connection("https://discordapp.com/api");
                // conn->SetTimeout(5);
                // conn->SetUserAgent("NirvanaBot (none, 0.1)");
                // conn->FollowRedirects(true);

                // RestClient::HeaderFields headers;
                // headers["Authorization"] = "Bot " + discord_token;
                // conn->SetHeaders(headers);
                // RestClient::Response r = conn->post("/channels/294187421858529280/messages", "{ \"content\": \"not bad for a test message\"}");
                // std::cout << std::endl << std::endl << std::endl << r.body << std::endl << std::endl;
                // RestClient::disable();