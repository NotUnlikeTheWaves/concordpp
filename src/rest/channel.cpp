#include "rest_client.h"

void rest_client::create_message(std::string channel, std::string message) {
	json arg;
	arg["content"] = message;
	this->api_call("/channels/" + channel + "/messages", POST, &arg);
}