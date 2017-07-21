#include <json.hpp>
#include "gateway_client.h"
#include "internal/debug.h"

using namespace concordpp;
using json = nlohmann::json;

gateway_client::gateway_client(std::string token) {
    this->token = token;
    socket = NULL;
    gateway_connect_state = NORMAL;
}

gateway_client::~gateway_client() {
    if(socket != NULL) {
        delete socket;
    }
}

void gateway_client::connect() {
    bool reconnect = false;
        // should move to session struct
    session_id = "";
    last_sequence_number = 0;
    while(gateway_connect_state != CLOSE) {
        debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Setting up gateway client");
        if(reconnect == false) {
            debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Gateway route: normal");
            socket = new web_socket(&(this->token), &cb_handler);
        } else {
            debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Gateway route: reconnecting with session id: " + session_id);
            socket = new web_socket(&(this->token), &cb_handler, web_socket::connection_type::RECONNECT);
            socket->set_session(session_id, last_sequence_number);
        }
        debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Starting gateway client");
        socket->start();
        debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Client stopped.");
            // Instructions reached when socket is exited
        if(gateway_connect_state != CLOSE) {
            session_id = socket->get_session_id();
            last_sequence_number = socket->get_sequence_number();
            reconnect = true;
        }
    }
}

void gateway_client::stop() {
    gateway_connect_state = CLOSE;
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
