#include "rest_client.h"
#include "internal/debug.h"
#include <thread>
#include <chrono>
    // Max interval between requests in milliseconds
#define CONCORDPP_MAX_REQUEST_INTERVAL 60000

using namespace concordpp::rest;
using json = nlohmann::json;

void rest_client::api_call(std::string uri, rest_request_type method, http_callback callback, nlohmann::json argument) {
    std::thread t = std::thread(std::bind(&rest_client::perform_request, this, uri, method, callback, argument));
    t.detach();
}

void rest_client::perform_request(std::string uri, rest_request_type method, http_callback callback, nlohmann::json argument) {
    http_thread_count++;
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
        // Timeout, keep trying.
    if(response.code == 28) {
        perform_request(uri, method, callback, argument);
        return;
    }
    try {
        json message = json::parse(response.body);
        if(response.code == 429) {  // Being rate limited
            int wait_for = message["retry_after"].get<int>();
            debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::REST, "API request exceeds rate limts. Retrying in " + std::to_string(wait_for) + "ms");
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_for));
            perform_request(uri, method, callback, argument);
        }
        else callback(response.code, message);
    } catch(const nlohmann::json::parse_error e) {
        std::string error = "Trouble: JSON Parse Error (API CALL):\n" \
        + std::to_string(response.code) + "\n" \
        + response.body + "\n";
        for(auto e : response.headers) {
            error = error + e.first + " - " + e.second + "\n";
        }
        debug::log(debug::log_level::IMPORTANT, debug::log_origin::REST, error);
    }
    http_thread_count--;
}
