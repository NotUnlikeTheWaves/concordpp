#include "rest_client.h"

using namespace concordpp;

rest_client::rest_client(std::string token) {
    this->token = token;
    RestClient::init();
    http_conn = new RestClient::Connection("https://discordapp.com/api");
    http_conn->SetTimeout(5);
    http_conn->FollowRedirects(true);
    http_conn->SetUserAgent("NirvanaBot (none, 0.1)");
    RestClient::HeaderFields headers;
    headers["Authorization"] = "Bot " + token;
    http_conn->SetHeaders(headers);
}

rest_client::~rest_client() {
    RestClient::disable();
    delete http_conn;
}
