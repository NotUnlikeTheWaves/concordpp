#include "gateway/callback_handler.h"

void callback_handler::add_callback(std::string event_name, std::function<void(nlohmann::json)> callback) {
    event_callback * cb = new event_callback(event_name, callback);
    callbacks.push_back(cb);
}

void callback_handler::handle_event(std::string event_name, json data) {
    for(auto iter = callbacks.begin(); iter != callbacks.end(); iter++) {
        if(event_name.compare((*iter)->get_event()) == 0)
        (*iter)->call(data);
    }
}
