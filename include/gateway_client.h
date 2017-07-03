#ifndef GATEWAY_CLIENT_H
#define GATEWAY_CLIENT_H

#include <string>

#include "gateway/web_socket.h"
#include "gateway/callback_handler.h"

class gateway_client {
public:
    gateway_client(std::string token, bool auto_connect = true);
    ~gateway_client();
    void connect();

        // Event handler/callback manager
    void add_callback(std::string event_name, std::function<void(json)> callback);
        // ========== gateway methods ========== //


private:
    std::string token;
    web_socket *socket;
    callback_handler cb_handler;
};

#endif
