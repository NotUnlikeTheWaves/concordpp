#include "gateway_client.h"
#include "rest_client.h"
#include <json.hpp>
#include <iostream>
int quit = 0;

void on_message(nlohmann::json data) {
    std::cout << "Message in on_message" << std::endl;
}

int main(int argc, char* argv[]) {
    gateway_client *d_gateway = new gateway_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    rest_client *d_rest = new rest_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    d_rest->create_message("319210580865187860", "Bootup sequence");
    d_gateway->add_callback("MESSAGE_CREATE", on_message);
    char d = '_';
    std::cin >> d;
    // while(quit == 0) {}
    delete d_gateway;
    delete d_rest;
}
