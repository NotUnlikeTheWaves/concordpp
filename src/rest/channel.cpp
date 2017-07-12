#include "rest_client.h"

using namespace concordpp;
using json = nlohmann::json;

void rest_client::create_message(snowflake channel, std::string message, http_callback callback) {
	json arg;
	arg["content"] = message;
	this->api_call("/channels/" + channel + "/messages", POST, callback, &arg);
}
