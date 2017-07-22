#ifndef GATEWAY_WEB_SOCKET_H
#define GATEWAY_WEB_SOCKET_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/thread/thread.hpp>
#include "gateway/callback_handler.h"

namespace concordpp {
    namespace gateway {
        typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;
        typedef websocketpp::client<websocketpp::config::asio_tls_client> web_socket_client;
        typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;
        typedef websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection_ptr;
        typedef uint16_t ws_close_code;
        class web_socket {
        public:
            enum connection_type {
                NORMAL,     // Send identify
                RECONNECT,  // Try resume, otherwise identify
            };

            web_socket(std::string *token, callback_handler *cb_handler, connection_type route = NORMAL);
            ~web_socket();
            void start();
            void stop(ws_close_code = websocketpp::close::status::normal);
            void send(std::string data);
            void set_session(std::string session_id, int sequence_number);
            std::string get_session_id();
            int get_sequence_number();
        private:
            std::string *token; // Keep pointer to token in gateway_client.h
            std::string session_id;
            int last_sequence_number;
            int heartbeat_interval;
            bool heartbeat_received;
            connection_type connection_route;
            bool heartbeat_is_running;
            boost::thread *heartbeat_thread;

            callback_handler *cb_handler;

            void identify();
            void resume();
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
    }
}
#endif
