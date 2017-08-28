#ifndef COMMAND_H
#define COMMAND_H

#include "gateway/callback.h"

namespace concordpp {
    namespace gateway {
        class command_callback : public event_callback {
        public:
            command_callback(std::string command, std::function<void(nlohmann::json)> callback);
            virtual void call(nlohmann::json argument);
        private:
            std::string command;
            int length;
        };
    }
}

#endif
