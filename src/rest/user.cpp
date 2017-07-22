#include "rest_client.h"

using namespace concordpp::rest;
using json = nlohmann::json;

void rest_client::get_current_user_guilds(http_callback callback) {
    this->api_call("/users/@me/guilds", GET, callback);
}
