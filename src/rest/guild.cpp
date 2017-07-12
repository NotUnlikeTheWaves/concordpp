#include "rest_client.h"

using namespace concordpp;
using json = nlohmann::json;

void rest_client::get_guild_channels(snowflake guild, http_callback callback) {
    this->api_call("/guilds/" + guild + "/channels", GET, callback);
}
