#include <iostream>
#include <json.hpp>
#include "gateway_client.h"

using namespace concordpp;
using json = nlohmann::json;

gateway_client::gateway_client(std::string token) {
    this->token = token;
    socket = NULL;
    std::cout << "Done init client" << std::endl;
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

void gateway_client::stop() {
    socket->stop();
}

void gateway_client::add_callback(std::string event_name, std::function<void(json)> callback) {
    cb_handler.add_callback(event_name, callback);
}

void gateway_client::set_status(std::string playing, std::time_t idle_since) {
    json data;
    data["op"] = 3;
    if(idle_since == -1) data["d"]["idle_since"] = NULL;
    else data["d"]["idle_since"] = idle_since;
    data["d"]["game"]["name"] = playing;
    socket->send(data.dump());
}
