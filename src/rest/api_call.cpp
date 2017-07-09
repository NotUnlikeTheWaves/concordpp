#include "rest_client.h"
#include <iostream>

using namespace concordpp;
using json = nlohmann::json;

void rest_client::api_call(std::string uri, rest_request_type method, http_callback callback, json *argument) {
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
    callback(response.code, nlohmann::json::parse(response.body));
}
