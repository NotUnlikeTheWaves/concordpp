#ifndef CALLBACK_HANDLER_H
#define CALLBACK_HANDLER_H
#include "gateway/callback.h"
#include <vector>
class callback_handler {
public:
    // ~callback_handler();
    void add_callback(std::string event_name, std::function<void(json)> callback);
    void handle_event(std::string event_name, json data);
private:
    std::vector<event_callback*> callbacks;
};

#endif
