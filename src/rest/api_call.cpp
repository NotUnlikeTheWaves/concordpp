#include "rest_client.h"
#include <iostream>
void rest_client::api_call(std::string uri, rest_request_type method, json *argument) {
	switch(method) {
		case GET:
			http_conn->get(uri);
			break;
		case POST:
			http_conn->post(uri, argument->dump());
			break;
		case PUT:
			http_conn->put(uri, argument->dump());
			break;
		case DELETE:
			http_conn->del(uri);
			break;
	}
}