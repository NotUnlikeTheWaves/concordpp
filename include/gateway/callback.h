#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <json.hpp>

using json = nlohmann::json;

class event_callback {
public:
    event_callback(std::string event, std::function<void(json)> callback);
    void call(json argument);
    std::string get_event();
private:
        // The event that triggers this callback being fired.
    std::string event;
    std::function<void(json)> callback;
};

#endif
