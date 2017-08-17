#pragma once
#include <functional>
#include <json.hpp>

namespace concordpp {
    namespace gateway {
        class event_callback {
        public:
            event_callback(std::string event, std::function<void(nlohmann::json)> callback);
            virtual void call(nlohmann::json argument);
            std::string get_event();
        private:
                // The event that triggers this callback being fired.
            std::string event;
            std::function<void(nlohmann::json)> callback;
        };
    }
}

