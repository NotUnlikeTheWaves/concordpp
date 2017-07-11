#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>
#include <json.hpp>
#include <functional>

#include "rest/api_call.h"

namespace concordpp {
    class rest_client {
            // int = HTTP code (e.g. 200, 204, etc)
            // nlohman::json = the response data
        typedef std::function<void(int, nlohmann::json)> http_callback;
    public:
        rest_client(std::string token);
        ~rest_client();

            // ========== API methods ========== //
            // Channel
        void create_message(std::string channel, std::string message, http_callback callback = [](int, nlohmann::json){});

            // Guild
        void get_guild_channels(std::string guild_snowflake, http_callback callback = [](int, nlohmann::json){});

            // User
        void get_current_user_guilds(http_callback callback = [](int, nlohmann::json){});
    private:
        std::string token;
        RestClient::Connection *http_conn;
        void api_call(std::string uri, rest_request_type method, http_callback callback, nlohmann::json *argument = NULL);
    };
}

#endif
