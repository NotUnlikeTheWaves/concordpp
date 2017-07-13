#include "rest_client.h"

using namespace concordpp;

rest_client::rest_client(std::string token) {
    this->token = token;
    http_thread_count = 0;
    RestClient::init();
}

rest_client::~rest_client() {
        // Wait for all HTTP threads to finish
    while(http_thread_count > 0) {}
    RestClient::disable();
}
