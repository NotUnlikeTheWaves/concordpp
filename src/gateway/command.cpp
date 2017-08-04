#include "gateway/command.h"

using namespace concordpp::gateway;
using json = nlohmann::json;

command_callback::command_callback(std::string command, std::function<void(nlohmann::json)> callback) : event_callback("MESSAGE_CREATE", callback) {
    this->command = command;
    this->length = command.length();
}

void command_callback::call(nlohmann::json argument) {
    std::string arg = argument["content"];
    std::string cmd = "";
    try {
        cmd = arg.substr(0, length);
        if(cmd == command) {
                // check if either all that is entered = command, or if there follows more after a whitespace.
            if(length == arg.length() || arg[length] == ' ') {
                std::string remainder = "";
                try {
                    remainder = arg.substr(length + 1, std::string::npos);
                } catch (const std::out_of_range &e) {
                    remainder = "";
                }
                argument["content"] = remainder;
                event_callback::call(argument);
            }
        }
    } catch (const std::out_of_range& e) {} // This is fine.
}
