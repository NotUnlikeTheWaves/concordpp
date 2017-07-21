#ifndef GATEWAY_CLIENT_H
#define GATEWAY_CLIENT_H

#include <string>
#include "gateway/web_socket.h"
#include "gateway/callback_handler.h"

namespace concordpp {
    class gateway_client {
    public:
        gateway_client(std::string token);
        ~gateway_client();
        void connect();
        void stop();

            // Event handler/callback manager
        void add_callback(std::string event_name, std::function<void(nlohmann::json)> callback);

            // ========== gateway methods ========== //
        void set_status(std::string playing, std::time_t idle_since = -1);

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

#endif
