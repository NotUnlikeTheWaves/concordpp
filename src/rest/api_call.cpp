#include "rest_client.h"
#include <iostream>
    // Max interval between requests in milliseconds
#define CONCORDPP_MAX_REQUEST_INTERVAL 60000

using namespace concordpp;
using json = nlohmann::json;

void rest_client::api_call(std::string uri, rest_request_type method, http_callback callback, nlohmann::json argument) {
    http_thread_count++;
    boost::thread t = boost::thread(boost::bind(&rest_client::perform_request, this, uri, method, callback, argument));
    t.detach();
}

void rest_client::perform_request(std::string uri, rest_request_type method, http_callback callback, nlohmann::json argument) {
    RestClient::Connection rest_conn = RestClient::Connection("https://discordapp.com/api");
    rest_conn.SetTimeout(5);
    rest_conn.FollowRedirects(true);
    rest_conn.SetUserAgent("NirvanaBot (none, 0.1)");
    rest_conn.AppendHeader("Authorization", "Bot " + token);
    RestClient::Response response;
    switch(method) {
        case GET:
            response = rest_conn.get(uri);
            break;
        case POST:
            response = rest_conn.post(uri, argument.dump());
            break;
        case PUT:
            response = rest_conn.put(uri, argument.dump());
            break;
        case DELETE:
            response = rest_conn.del(uri);
            break;
    }
    callback(response.code, nlohmann::json::parse(response.body));
    http_thread_count--;
}
