#include "client.h"
#include "api.h"

int main(int argc, char* argv[]) {
    discordClient *client;
    
    try {
        client = new discordClient("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
        std::cout << "Client created. Type something/hit enter to connect." << std::endl;
        char keep;
        std::cin >> keep;
        client->connect();
        std::cout << "Connection to Discord started." << std::endl << std::endl << std::endl;
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
    
    HttpAPIHandler *handler = new HttpAPIHandler("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    handler->CreateMessage(319210580865187860, "Test message");

    char end;
    std::cin >> end;
    delete handler;
    delete client;
    std::cout << "end of program" << std::endl;
}
