#include "rest_client.h"

using namespace concordpp;
using json = nlohmann::json;

void rest_client::get_guild_channels(std::string guild_snowflake, http_callback callback) {
    this->api_call("/guilds/" + guild_snowflake + "/channels", GET, callback);
}
