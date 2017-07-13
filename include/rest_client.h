#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <functional>
#include <chrono>
#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>
#include <json.hpp>

#include "rest/api_call.h"

namespace concordpp {
    class rest_client {
            // int = HTTP code (e.g. 200, 204, etc)
            // nlohman::json = the response data
        typedef std::function<void(int, nlohmann::json)> http_callback;
        typedef std::string snowflake;
    public:
        rest_client(std::string token);
        ~rest_client();

            // ========== API methods ========== //
            // Channel
        void create_message(snowflake channel, std::string message, http_callback callback = [](int, nlohmann::json){});

            // Guild
        void get_guild_channels(snowflake guild, http_callback callback = [](int, nlohmann::json){});

            // User
        void get_current_user_guilds(http_callback callback = [](int, nlohmann::json){});
    private:
        std::string token;
        RestClient::Connection *http_conn;
        uint64_t last_request;

            // Base call for all HTTP API requests
        void api_call(std::string uri, rest_request_type method, http_callback callback, nlohmann::json *argument = NULL);
        void perform_request(std::string uri, rest_request_type method, http_callback callback, nlohmann::json *argument = NULL);
        void open_connection();
        void reset_connection();
    };
}

#endif
