#ifndef CALLBACK_HANDLER_H
#define CALLBACK_HANDLER_H
#include "gateway/callback.h"
#include "gateway/command.h"
#include <vector>
namespace concordpp {
    namespace gateway {
        class callback_handler {
        public:
            ~callback_handler();
            void add_callback(std::string event_name, std::function<void(nlohmann::json)> callback);
            void add_command(std::string command, std::function<void(nlohmann::json)> callback);
            void handle_event(std::string event_name, nlohmann::json data);
        private:
            std::vector<event_callback*> callbacks;
        };
    }
}

#endif
