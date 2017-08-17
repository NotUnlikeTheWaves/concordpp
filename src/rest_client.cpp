#include "rest_client.h"
#include <thread>
#include <chrono>

using namespace concordpp::rest;

rest_client::rest_client(std::string token) {
    this->token = token;
    http_thread_count = 0;
    RestClient::init();
}

rest_client::~rest_client() {
    // Wait for all HTTP threads to finish
    // Use sleep here so we don't do 100% cpu doing nothing but waiting. - Justasic
    while(http_thread_count > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    RestClient::disable();
}
