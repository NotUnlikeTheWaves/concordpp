#include "gateway_client.h"
#include <iostream>
gateway_client::gateway_client(std::string token, bool auto_connect) {
    this->token = token;
    socket = NULL;
    std::cout << "Done init client" << std::endl;
    if(auto_connect == true) {
        std::cout << "Connect WS" << std::endl;
        connect();
    }
}

gateway_client::~gateway_client() {
    if(socket != NULL) {
        delete socket;
    }
}

void gateway_client::connect() {
    std::cout << "Connect function" << std::endl;
    socket = new web_socket(&(this->token), &cb_handler);
    socket->start();
}

void gateway_client::add_callback(std::string event_name, std::function<void(json)> callback) {
    cb_handler.add_callback(event_name, callback);
}
