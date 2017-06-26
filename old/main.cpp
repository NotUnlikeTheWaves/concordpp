#include "client.h"
#include "rest/webapi.h"
#include <json.hpp>
void getMsg(nlohmann::json data);

HttpAPIHandler *handler;

int main(int argc, char* argv[]) {
    discordClient *client;
    
    try {
        client = new discordClient("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
        std::cout << "Client created. Type something/hit enter to connect." << std::endl;
        char keep;
        std::cin >> keep;
        client->connect();
        client->events.addRawHandler(getMsg);
        std::cout << "Connection to Discord started." << std::endl << std::endl << std::endl;
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
    
    handler = new HttpAPIHandler("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");

    char end;
    std::cin >> end;
    delete handler;
    delete client;
    std::cout << "end of program" << std::endl;
}


void getMsg(nlohmann::json data) {
    if(data["t"] == "MESSAGE_CREATE") {
        if(data["d"]["content"] == "test me") {
            std::string author = data["d"]["author"]["username"];
            std::string channel_id = data["d"]["channel_id"];
            handler->CreateMessage(channel_id, "It's not broken at least, " + author);
        }
    }
}
