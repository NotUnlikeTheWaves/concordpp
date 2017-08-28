#pragma once

#include <string>
#include "gateway/web_socket.h"
#include "gateway/callback_handler.h"

namespace concordpp {
    namespace gateway {
        namespace status_types {
            typedef std::string status;
            static const status ONLINE = "online";
            static const status DO_NOT_DISTURB = "dnd";
            static const status IDLE = "idle";
            static const status INVISIBLE = "invisible";
            static const status OFFLINE = "offline";
        }
        class gateway_client {
            typedef std::string snowflake;
        public:
            gateway_client(std::string token);
            ~gateway_client();
            void connect();
            void stop();

                // Event handler/callback manager
            void add_callback(std::string event_name, std::function<void(nlohmann::json)> callback);
            void add_command(std::string command_name, std::function<void(nlohmann::json)> callback);

                // ========== gateway methods ========== //
            void set_status(concordpp::gateway::status_types::status status, std::string playing, bool afk = false, std::time_t idle_since = -1);
            void request_guild_members(snowflake guild, std::string starts_with = "", int limit = 0);

        private:
            enum connection_state {
                NORMAL,     // Send identify
                RECONNECT,  // Try resume, otherwise identify
                CLOSE,      // Don't reconnect
            };
            connection_state gateway_connect_state;
            std::string token;
            std::string session_id;
            int last_sequence_number;
            web_socket *socket;
            callback_handler cb_handler;
        };
    }
}

