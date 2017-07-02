#ifndef GATEWAY_WEB_SOCKET_H
#define GATEWAY_WEB_SOCKET_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/thread/thread.hpp>

#include "gateway/callback_handler.h"

typedef websocketpp::client<websocketpp::config::asio_tls_client> web_socket_client;
typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;
typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;
typedef websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection_ptr;

class web_socket {
public:
    web_socket(std::string *token, callback_handler *cb_handler);
    ~web_socket();
    void start();
private:
    std::string *token; // Keep pointer to token in gateway_client.h
    int last_sequence_number;
    int heartbeat_interval;

    boost::thread *heartbeat_thread;
    boost::thread *run_thread;

    callback_handler *cb_handler;

    void threaded_run();
    void send_heartbeat();

        // websocketpp
    web_socket_client client;
    websocketpp::connection_hdl persistent_hdl;

    void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
    void on_close(websocketpp::connection_hdl);
    void on_open(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
    void on_socket_init(websocketpp::connection_hdl);
    context_ptr on_tls_init(websocketpp::connection_hdl);
};

#endif
