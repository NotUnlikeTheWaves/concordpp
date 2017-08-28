#pragma once
#include <string>
namespace concordpp {
    namespace debug {
        enum log_level {
            ALL,
            INFORMATIONAL,
            IMPORTANT,
            NONE,
        };
        enum log_origin {
            GATEWAY,
            REST,
            OTHER,
        };
        void set_log_level(log_level level);
        log_level get_log_level();
        void log(log_level importance, log_origin origin, std::string event);
    }
}

