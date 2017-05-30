#include <client.h>

int main(int argc, char* argv[]) {
    std::string uri = "wss://gateway.discord.gg/?v=5&encoding=json";
    discordClient *client;
    if (argc == 2) {
        uri = argv[1];
    }

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
    //while(1);
    char end;
    std::cin >> end;
    delete client;
    std::cout << "end of program" << std::endl;
}
