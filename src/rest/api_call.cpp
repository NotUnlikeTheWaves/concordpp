#include "rest_client.h"
#include <iostream>
    // Max interval between requests in milliseconds
#define CONCORDPP_MAX_REQUEST_INTERVAL 60000

using namespace concordpp;
using json = nlohmann::json;

void rest_client::api_call(std::string uri, rest_request_type method, http_callback callback, json *argument) {
        // Get current time
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if((now - last_request) > CONCORDPP_MAX_REQUEST_INTERVAL) {
        reset_connection();
        std::cout << "Resetting HTTP/REST connection to avoid timeout." << std::endl;
    }
    perform_request(uri, method, callback, argument);
}

void rest_client::perform_request(std::string uri, rest_request_type method, http_callback callback, nlohmann::json *argument) {
    RestClient::Response response;
    switch(method) {
        case GET:
            response = http_conn->get(uri);
            break;
        case POST:
            response = http_conn->post(uri, argument->dump());
            break;
        case PUT:
            response = http_conn->put(uri, argument->dump());
            break;
        case DELETE:
            response = http_conn->del(uri);
            break;
    }
    last_request = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    callback(response.code, nlohmann::json::parse(response.body));
}
