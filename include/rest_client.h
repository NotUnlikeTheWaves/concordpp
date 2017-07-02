#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>
#include <json.hpp>

#include "rest/api_call.h"

using json = nlohmann::json;

class rest_client {
public:
	rest_client(std::string token);
	~rest_client();

		// ========== API methods ========== //
		// Channel
	void create_message(std::string channel, std::string message);


private:
	std::string token;
	RestClient::Connection *http_conn;
	void api_call(std::string uri, rest_request_type method, json *argument = NULL);
};

#endif
