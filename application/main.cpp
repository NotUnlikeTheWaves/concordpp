#include "gateway_client.h"
#include "rest_client.h"
#include <json.hpp>
#include <iostream>
int main(int argc, char* argv[]) {
    gateway_client *d_gateway = new gateway_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    rest_client *d_rest = new rest_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    d_rest->create_message("319210580865187860", "Bootup sequence");
    char d = '_';
    std::cin >> d;
    delete d_gateway;
    delete d_rest;
}
