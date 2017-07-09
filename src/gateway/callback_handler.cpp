#include "gateway/callback_handler.h"

using json = nlohmann::json;

callback_handler::~callback_handler() {
    while(callbacks.size() > 0) {
        event_callback *cb = callbacks.back();
        callbacks.pop_back();
        delete cb;
    }
}

void callback_handler::add_callback(std::string event_name, std::function<void(json)> callback) {
    event_callback * cb = new event_callback(event_name, callback);
    callbacks.push_back(cb);
}

void callback_handler::handle_event(std::string event_name, json data) {
    for(auto iter = callbacks.begin(); iter != callbacks.end(); iter++) {
        if(event_name.compare((*iter)->get_event()) == 0)
        (*iter)->call(data);
    }
}
