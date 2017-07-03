#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <json.hpp>
#include <iostream>

#include "gateway/web_socket.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using json = nlohmann::json;

web_socket::web_socket(std::string *token, callback_handler *cb_handler) {
    this->token = token;
    this->cb_handler = cb_handler;
    last_sequence_number = 0;
    heartbeat_thread = NULL;
    run_thread = NULL;

    client.init_asio();
    client.set_access_channels(websocketpp::log::alevel::all); //check
    client.set_error_channels(websocketpp::log::elevel::all);

    // Register our handlers
    client.set_socket_init_handler(bind(&web_socket::on_socket_init,this,::_1));
    client.set_tls_init_handler(bind(&web_socket::on_tls_init,this,::_1));
    client.set_message_handler(bind(&web_socket::on_message,this,::_1,::_2));
    client.set_open_handler(bind(&web_socket::on_open,this,::_1));
    client.set_close_handler(bind(&web_socket::on_close,this,::_1));
    client.set_fail_handler(bind(&web_socket::on_fail,this,::_1));
    client.set_user_agent("");

    std::string uri = "wss://gateway.discord.gg/?v=5&encoding=json";
    websocketpp::lib::error_code ec;
    web_socket_client::connection_ptr con = client.get_connection(uri, ec);

    if (ec) {
        client.get_alog().write(websocketpp::log::alevel::app,ec.message());
        return;
    }

    client.connect(con);
}

web_socket::~web_socket() {
    if(heartbeat_thread != NULL) {
        heartbeat_thread->interrupt();
        heartbeat_thread->join();
    }
    client.close(persistent_hdl, websocketpp::close::status::normal, "");
    run_thread->join();
}

void web_socket::start() {
    run_thread = new boost::thread(
        boost::bind(&web_socket::threaded_run, this)
    );
}

void web_socket::threaded_run() {
    client.run();
}

void web_socket::on_socket_init(websocketpp::connection_hdl) {
    std::cout << "Socket init." << std::endl;
}

context_ptr web_socket::on_tls_init(websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
        ctx->set_verify_mode(0);    // check if needed
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    ctx->set_verify_mode(0);
    return ctx;
}

void web_socket::on_fail(websocketpp::connection_hdl hdl) {
    web_socket_client::connection_ptr con = client.get_con_from_hdl(hdl);
    std::cout << "Fail handler" << std::endl;
    std::cout << con->get_state() << std::endl;
    std::cout << con->get_local_close_code() << std::endl;
    std::cout << con->get_local_close_reason() << std::endl;
    std::cout << con->get_remote_close_code() << std::endl;
    std::cout << con->get_remote_close_reason() << std::endl;
    std::cout << con->get_ec() << " - " << con->get_ec().message() << std::endl;
}

void web_socket::on_open(websocketpp::connection_hdl hdl) {
    std::cout << "Connection opened." << std::endl;
}
void web_socket::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "Message received!" << std::endl;
    std::string message = msg->get_payload();
    std::cout << "Message gotten!" << std::endl;
    std::cout << message << std::endl;
    std::cout << "Message printed!" << std::endl;
    json json_msg = json::parse(message);

    if(!json_msg["s"].is_null()) {
            std::cout << "S is NOT NULL" << std::endl;
            last_sequence_number = json_msg["s"].get<int>();
    } else std::cout << "S == NULL !!!" << std::endl;

    switch(json_msg["op"].get<int>()) {
        case 0 : {
            std::string event = json_msg["t"];
            if(event == "READY") {
                std::cout <<  "Ready event!" << std::endl;
                persistent_hdl = hdl;
                heartbeat_thread = new boost::thread(boost::bind(&web_socket::send_heartbeat, this));
            }
            cb_handler->handle_event(event, json_msg["d"]);
            break;
        }
        case 10: {
            heartbeat_interval = json_msg["d"]["heartbeat_interval"].get<int>();
            std::cout << std::endl << "IDENTIFY NOW!" << std::endl << std::endl;
            json identify;
            identify["op"] = 2;
            identify["d"]["token"] = (*token);
            identify["d"]["properties"]["$os"] = "Linux";
            identify["d"]["compress"] = false;
            identify["d"]["large_threshold"] = 50;
            std::cout << "Token from memory: " << *token << std::endl;
            //identify["d"]["shard"] = {1, 1};
            client.send(hdl, identify.dump(), websocketpp::frame::opcode::text);
            break;
        }
        default: {
            std::cout << "Unimplemented opcode: " << json_msg["op"] << std::endl;
            break;
        }
    }


}

void web_socket::on_close(websocketpp::connection_hdl) {
    std::cout << "WS Closing." << std::endl;
}

void web_socket::send_heartbeat() {
    json heartbeat;
    heartbeat["op"] = 1;    // Heartbeat OP
    while(true) {
        heartbeat["d"] = last_sequence_number;
        client.send(persistent_hdl, heartbeat.dump(), websocketpp::frame::opcode::text);
        std::cout << "Sent heartbeat" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(heartbeat_interval));
    }
}
