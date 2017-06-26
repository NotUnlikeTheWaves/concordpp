#ifndef GATEWAY_CLIENT_H
#define GATEWAY_CLIENT_H

#include <string>

#include "gateway/web_socket.h"

class gateway_client {
public:
    gateway_client(std::string token, bool auto_connect = true);
    ~gateway_client();
    void connect();
private:
    std::string token;
    web_socket *socket;
};

#endif
