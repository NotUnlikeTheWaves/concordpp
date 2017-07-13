#include "rest_client.h"

using namespace concordpp;

rest_client::rest_client(std::string token) {
    this->token = token;
    open_connection();
}

rest_client::~rest_client() {
    RestClient::disable();
    delete http_conn;
}

void rest_client::open_connection() {
    RestClient::init();
    http_conn = new RestClient::Connection("https://discordapp.com/api");
    http_conn->SetTimeout(5);
    http_conn->FollowRedirects(true);
    http_conn->SetUserAgent("NirvanaBot (none, 0.1)");
    http_conn->AppendHeader("Authorization", "Bot " + token);
    last_request = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void rest_client::reset_connection() {
    delete http_conn;
    open_connection();
}
