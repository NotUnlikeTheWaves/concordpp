#ifndef WSCONN_H
#define WSCONN_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/thread/thread.hpp>

#include "eventhandler.h"

typedef websocketpp::client<websocketpp::config::asio_tls_client> _client;
typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;
typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;
typedef websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection_ptr;
//typedef websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection_ptr;

class discordWebSocket {
public:
	typedef discordWebSocket type;
	discordWebSocket(std::string token);
	discordWebSocket(const char *c){ discordWebSocket(std::string(c)); }
	discordWebSocket(); // empty constructor
	~discordWebSocket();
	void setEventHandler(eventHandler *event_handler);
	static void *run(void *ptr);
private:
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	void on_close(websocketpp::connection_hdl);
	void on_open(websocketpp::connection_hdl hdl);
	void on_fail(websocketpp::connection_hdl hdl);
	void on_socket_init(websocketpp::connection_hdl);
	context_ptr on_tls_init(websocketpp::connection_hdl);
	static void *heartbeat(void *ptr);

	_client ws_endpoint;
	int sequence_no;
	int heartbeat_interval;
	std::string discord_token;
	websocketpp::connection_hdl persistent_hdl;
	boost::thread *hb_thread;
	eventHandler *handler = NULL;
};
#endif
