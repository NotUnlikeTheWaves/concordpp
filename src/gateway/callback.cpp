#include "gateway/callback.h"

using json = nlohmann::json;

event_callback::event_callback(std::string event, std::function<void(json)> callback) {
    this->event = event;
    this->callback = callback;
}

void event_callback::call(json argument) {
    callback(argument);
}

std::string event_callback::get_event() {
    return event;
}
