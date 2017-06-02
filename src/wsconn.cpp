#include <wsconn.h>

#include <websocketpp/config/asio_client.hpp>

#include <websocketpp/client.hpp>

#include <json.hpp>

#include <iostream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using json = nlohmann::json;


discordWebSocket::discordWebSocket(std::string token) {
	discord_token = token;
    hb_thread = NULL;
	std::cout << "Try to connect: " << token << std::endl;
	// Initialize ASIO
    ws_endpoint.init_asio();

    ws_endpoint.set_access_channels(websocketpp::log::alevel::all); //check
    ws_endpoint.set_error_channels(websocketpp::log::elevel::all);

    // Register our handlers
    ws_endpoint.set_socket_init_handler(bind(&type::on_socket_init,this,::_1));
    ws_endpoint.set_tls_init_handler(bind(&type::on_tls_init,this,::_1));
    ws_endpoint.set_message_handler(bind(&type::on_message,this,::_1,::_2));
    ws_endpoint.set_open_handler(bind(&type::on_open,this,::_1));
    ws_endpoint.set_close_handler(bind(&type::on_close,this,::_1));
    ws_endpoint.set_fail_handler(bind(&type::on_fail,this,::_1));
    ws_endpoint.set_user_agent("");

    std::string uri = "wss://gateway.discord.gg/?v=5&encoding=json";
    websocketpp::lib::error_code ec;
    _client::connection_ptr con = ws_endpoint.get_connection(uri, ec);

    if (ec) {
        ws_endpoint.get_alog().write(websocketpp::log::alevel::app,ec.message());
        return;
    }

    ws_endpoint.connect(con);
}
discordWebSocket::discordWebSocket() {std::cout << "wrong websocket" << std::endl;}

discordWebSocket::~discordWebSocket() {
    std::cout << "Close websocket attempt." << std::endl;
    ws_endpoint.close(persistent_hdl, websocketpp::close::status::normal, "");
    if(hb_thread != NULL) { // heartbeat thread still running
        std::cout << "Kill heartbeat thread" << std::endl;
        hb_thread->interrupt();
        delete hb_thread;
        hb_thread = NULL;
    }
}

void *discordWebSocket::run(void *ptr) {
    discordWebSocket *dws = reinterpret_cast<discordWebSocket*>(ptr);
    dws->ws_endpoint.run();
}

void discordWebSocket::on_socket_init(websocketpp::connection_hdl) {
	std::cout << "Socket init." << std::endl;
}

context_ptr discordWebSocket::on_tls_init(websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
        ctx->set_verify_mode(0);	// check if needed
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    ctx->set_verify_mode(0);
    return ctx;
}

void discordWebSocket::on_fail(websocketpp::connection_hdl hdl) {
    _client::connection_ptr con = ws_endpoint.get_con_from_hdl(hdl);
    
    std::cout << "Fail handler" << std::endl;
    std::cout << con->get_state() << std::endl;
    std::cout << con->get_local_close_code() << std::endl;
    std::cout << con->get_local_close_reason() << std::endl;
    std::cout << con->get_remote_close_code() << std::endl;
    std::cout << con->get_remote_close_reason() << std::endl;
    std::cout << con->get_ec() << " - " << con->get_ec().message() << std::endl;
}

void discordWebSocket::on_open(websocketpp::connection_hdl hdl) {
    std::cout << "Connection opened." << std::endl;
}
void discordWebSocket::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "Message received!" << std::endl;
    std::string message = msg->get_payload();
    std::cout << "Message gotten!" << std::endl;
    std::cout << message << std::endl;
    std::cout << "Message printed!" << std::endl;

    json json_msg = json::parse(message);
    
    if(!json_msg["s"].is_null()) {
			std::cout << "S is NOT NULL" << std::endl;
			sequence_no = json_msg["s"].get<int>();
    } else std::cout << "S == NULL !!!" << std::endl;

    switch(json_msg["op"].get<int>()) {
    	case 0 : {
    		if(json_msg["t"] == "READY") {
    			std::cout <<  "Ready event!" << std::endl;
        		persistent_hdl = hdl;
                hb_thread = new boost::thread(heartbeat, (void *) this);
    		}
        	break;
    	}
    	case 10: {
    		heartbeat_interval = json_msg["d"]["heartbeat_interval"].get<int>();
    		std::cout << std::endl << "IDENTIFY NOW!" << std::endl << std::endl;
    		json identify;
    		identify["op"] = 2;
    		identify["d"]["token"] = discord_token;
    		identify["d"]["properties"]["$os"] = "Linux";
    		identify["d"]["compress"] = false;
    		identify["d"]["large_threshold"] = 50;
    		std::cout << "Token from memory: " << discord_token << std::endl;
    		//identify["d"]["shard"] = {1, 1};
			ws_endpoint.send(hdl, identify.dump(), websocketpp::frame::opcode::text);
    		break;
    	}
    	default: {
    		std::cout << "Unimplemented opcode: " << json_msg["op"] << std::endl;
    		break;
    	}
    }
}
void discordWebSocket::on_close(websocketpp::connection_hdl) {

    // std::cout << "Socket Init: " << std::chrono::duration_cast<dur_type>(m_socket_init-m_start).count() << std::endl;
    // std::cout << "TLS Init: " << std::chrono::duration_cast<dur_type>(m_tls_init-m_start).count() << std::endl;
    // std::cout << "Open: " << std::chrono::duration_cast<dur_type>(m_open-m_start).count() << std::endl;
    // std::cout << "Message: " << std::chrono::duration_cast<dur_type>(m_message-m_start).count() << std::endl;
    // std::cout << "Close: " << std::chrono::duration_cast<dur_type>(m_close-m_start).count() << std::endl;
    std::cout << "WS Closing." << std::endl;
}

void *discordWebSocket::heartbeat(void *ptr) {
    discordWebSocket *dws = reinterpret_cast<discordWebSocket*>(ptr);
	while(1) {
		json heartbeat;
		heartbeat["op"] = 1;
		heartbeat["d"] = dws->sequence_no;
		dws->ws_endpoint.send(dws->persistent_hdl, heartbeat.dump(), websocketpp::frame::opcode::text);
		std::cout << "Sent HEARTBEAT !!\nSleeping for: " << dws->heartbeat_interval << std::endl;
		//usleep(1000 * dws->heartbeat_interval);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(dws->heartbeat_interval));
	}
}
