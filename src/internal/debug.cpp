#include <iostream>
#include "internal/debug.h"
using namespace concordpp::debug;

namespace concordpp {
    namespace debug {

        static log_level debug_level = NONE;

        void set_log_level(log_level level) {
            debug_level = level;
        }

        log_level get_log_level() {
            return debug_level;
        }

        void log(log_level importance, log_origin origin, std::string event) {
            if(importance >= debug_level) {
                std::string location;
                switch(origin) {
                    case GATEWAY:
                        location = "gateway";
                        break;
                    case REST:
                        location = "rest";
                        break;
                    case OTHER:
                        location = "other";
                        break;
                }
                std::cout << "[" << location << "] " << event << std::endl;
            }
        }

    }
}
