#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <json.hpp>
#include "gateway/web_socket.h"
#include "internal/debug.h"

using namespace concordpp::gateway;
using json = nlohmann::json;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

web_socket::web_socket(std::string *token, callback_handler *cb_handler, connection_type route) {
    this->token = token;
    this->cb_handler = cb_handler;
    last_sequence_number = 0;
    heartbeat_thread = NULL;
    connection_route = route;

    client.init_asio();
    client.set_access_channels(websocketpp::log::alevel::none);
    client.set_error_channels(websocketpp::log::elevel::none);

    // Register our handlers
    client.set_socket_init_handler(bind(&web_socket::on_socket_init,this,::_1));
    client.set_tls_init_handler(bind(&web_socket::on_tls_init,this,::_1));
    client.set_message_handler(bind(&web_socket::on_message,this,::_1,::_2));
    client.set_open_handler(bind(&web_socket::on_open,this,::_1));
    client.set_close_handler(bind(&web_socket::on_close,this,::_1));
    client.set_fail_handler(bind(&web_socket::on_fail,this,::_1));
    client.set_user_agent("");

    std::string uri = "wss://gateway.discord.gg/?v=6&encoding=json";
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
        if(heartbeat_is_running == true) {
            heartbeat_thread->interrupt();
            heartbeat_thread->join();
        }
        delete heartbeat_thread;
    }
}

void web_socket::start() {
    client.run();
}

void web_socket::stop(ws_close_code reason) {
    client.close(persistent_hdl, reason, "");
}

void web_socket::set_session(std::string session_id, int sequence_number) {
    last_sequence_number = sequence_number;
    this->session_id = session_id;
}

std::string web_socket::get_session_id() {
    return session_id;
}

int web_socket::get_sequence_number() {
    return last_sequence_number;
}

void web_socket::on_socket_init(websocketpp::connection_hdl) {
    debug::log(debug::log_level::ALL, debug::log_origin::GATEWAY, "on_socket_init() called");
}

context_ptr web_socket::on_tls_init(websocketpp::connection_hdl) {
    debug::log(debug::log_level::ALL, debug::log_origin::GATEWAY, "on_tls_init() called");
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
        ctx->set_verify_mode(0);    // check if needed
    } catch (std::exception& e) {
        std::string error = e.what();
        debug::log(debug::log_level::IMPORTANT, debug::log_origin::GATEWAY, ("Exception in TLS init: " + error));
    }
    ctx->set_verify_mode(0);
    return ctx;
}

void web_socket::on_fail(websocketpp::connection_hdl hdl) {
    web_socket_client::connection_ptr con = client.get_con_from_hdl(hdl);
    std::string fail_string = "Failure in web socket:\nState: " + std::to_string(con->get_state()) + "\n" \
    + "Local close code: " + std::to_string(con->get_local_close_code()) + "\n" \
    + "Local close reason: " + con->get_local_close_reason() + "\n" \
    + "Remote close code: " + std::to_string(con->get_remote_close_code()) + "\n" \
    + "Remote close reason: " + con->get_remote_close_reason() + "\n";
    + "Error msg: " + con->get_ec().message();
    debug::log(debug::log_level::IMPORTANT, debug::log_origin::GATEWAY, fail_string);
}

void web_socket::on_open(websocketpp::connection_hdl hdl) {
    persistent_hdl = hdl;
    debug::log(debug::log_level::ALL, debug::log_origin::GATEWAY, "on_open() called");
}
void web_socket::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    std::string message = msg->get_payload();
    json json_msg = json::parse(message);
    debug::log(debug::log_level::ALL, debug::log_origin::GATEWAY, "on_message called:\n" + json_msg.dump(4));

    if(!json_msg["s"].is_null()) {
            last_sequence_number = json_msg["s"].get<int>();
    }

    int op_code = json_msg["op"].get<int>();
    switch(json_msg["op"].get<int>()) {
        case 0: {   // dispatch
            std::string event = json_msg["t"];
            if(event == "READY") {  // note to self: move this to its own callback
                debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "READY event received");
                session_id = json_msg["d"]["session_id"];
            }
            cb_handler->handle_event(event, json_msg["d"]);
            break;
        }
        case 7: {   // reconnect
            stop(websocketpp::close::status::try_again_later); // stop everything, return to main loop in gateway client, then reconnect.
            return;
        }
        case 9: {
                // Can't connect properly, so just identify. Wait for 5 seconds according to discord reference.
            debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Failed to resume session. Identifying instead.");
            boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
            identify();
            break;
        }
        case 10: {  // hello
            debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Hello OPcode received, identifying");
            heartbeat_interval = json_msg["d"]["heartbeat_interval"].get<int>();
            heartbeat_thread = new boost::thread(boost::bind(&web_socket::send_heartbeat, this));
            switch(connection_route) {
                case NORMAL:
                    identify();
                    break;
                case RECONNECT:
                    resume();
                    break;
            }
            break;
        }
        case 11: {
            heartbeat_received = true;
            break;
        }
        default: {
            debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Unimplemented opcode: " + std::to_string(op_code));
            break;
        }
    }


}

void web_socket::identify() {
    json identify;
    identify["op"] = 2;
    identify["d"]["token"] = (*token);
    identify["d"]["properties"]["$os"] = "Linux";
    identify["d"]["compress"] = false;
    identify["d"]["large_threshold"] = 50;
    client.send(persistent_hdl, identify.dump(), websocketpp::frame::opcode::text);
}

void web_socket::resume() {
    json resume;
    resume["op"] = 6;
    resume["d"]["token"] = (*token);
    resume["d"]["session_id"] = session_id;
    resume["d"]["seq"] = last_sequence_number;
    client.send(persistent_hdl, resume.dump(), websocketpp::frame::opcode::text);
}

void web_socket::on_close(websocketpp::connection_hdl) {
    debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "on_close() called");
}

void web_socket::send_heartbeat() {
    heartbeat_is_running = true;
    heartbeat_received = true;
    json heartbeat;
    heartbeat["op"] = 1;    // Heartbeat OP
    while(true) {
        if(heartbeat_received == false) {
            stop(websocketpp::close::status::try_again_later);
            break;
        }
        heartbeat_received = false;
        heartbeat["d"] = last_sequence_number;
        client.send(persistent_hdl, heartbeat.dump(), websocketpp::frame::opcode::text);
        debug::log(debug::log_level::INFORMATIONAL, debug::log_origin::GATEWAY, "Sending heartbeat");
        boost::this_thread::sleep_for(boost::chrono::milliseconds(heartbeat_interval));
    }
    heartbeat_is_running = false;
}

void web_socket::send(std::string data) {
    client.send(persistent_hdl, data, websocketpp::frame::opcode::text);
}
