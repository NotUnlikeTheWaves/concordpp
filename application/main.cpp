#include "gateway_client.h"
#include <json.hpp>
#include <iostream>
int main(int argc, char* argv[]) {
    gateway_client *d_gateway = new gateway_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    char d = '_';
    std::cin >> d;
    delete d_gateway;
}
