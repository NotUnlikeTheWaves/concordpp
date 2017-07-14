#include "gateway_client.h"
#include "rest_client.h"
#include <json.hpp>
#include <iostream>
#include <ctime>

int quit = 0;
concordpp::rest_client *d_rest;
concordpp::gateway_client *d_gateway;

void on_message(nlohmann::json data) {
    if(data["content"] == "test me") {
        d_rest->create_message(data["channel_id"], "This is (not) a test");
    } else if(data["content"] == "shutdown") {
        d_gateway->stop();
        d_rest->create_message(data["channel_id"], "zzz", [](int code, nlohmann::json data) {});
    } else if(data["content"] == "channels") {
        std::string tempchan = data["channel_id"];
        std::string *channel = new std::string(tempchan);
        d_rest->get_current_user_guilds([channel](int code, nlohmann::json data){
            int *count = new int(data.size());
            std::string *response = new std::string("");
            for (nlohmann::json::iterator it = data.begin(); it != data.end(); ++it) {
                std::cout << *it << '\n'; // debug only!
                d_rest->get_guild_channels((*it)["id"], [channel, count, response] (int code, nlohmann::json data) {
                    std::cout << "In guild channel callback: " << (*count) << std::endl;
                    (*count) = (*count) - 1;
                    for(nlohmann::json::iterator it = data.begin(); it != data.end(); ++it) {
                        std::string chan_name = (*it)["name"];
                        (*response) = (*response) + chan_name + "\n";
                        std::cout << (*count) << std::endl;
                    }
                    if((*count) == 0) {
                        std::cout << "make message to channels: " << (*channel) << std::endl;
                        d_rest->create_message((*channel), "```json\n" + (*response) + "\n```", [](int code, nlohmann::json data) {
                            std::cout << "cm response: " << code << "\nalso: " << data.dump(4) << std::endl;
                        });
                        delete channel;
                        delete response;
                        delete count;
                    }
                });
            }
        });
    }
}

int main(int argc, char* argv[]) {
    d_gateway = new concordpp::gateway_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    d_rest = new concordpp::rest_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    d_gateway->add_callback("MESSAGE_CREATE", on_message);
    d_gateway->add_callback("MESSAGE_CREATE", [](nlohmann::json data){
        if(data["content"] == "lambda") {
            d_rest->create_message(data["channel_id"], "Lambda test done.");
        }
    });
    d_gateway->add_callback("MESSAGE_CREATE", [](nlohmann::json data) {
        std::string arg = data["content"];
        std::string cmd = arg.substr(0, arg.find(' '));
        if(cmd != arg && cmd == "game") {
            std::string remainder = arg.substr(arg.find(' ') + 1, std::string::npos);
            d_gateway->set_status(remainder, std::time(0));
        }
    });
    d_gateway->connect();   // Blocking call
    delete d_gateway;
    delete d_rest;
}
