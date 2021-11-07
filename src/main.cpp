//
// src/main.cpp
// Created by c0repwn3r on 7/11/21
//
// Distrubuted under the MIT License
//
// Written with <3 using vim

// libstdc++ imports
#include <iostream>
#include <cstdlib>
#include <functional>
#include <string>
#include <thread>

#include "box2d.h" // Import the box2d gluefile to import all of b2d

#include "starkingdoms.hpp" // Import the starkingdoms gluefile so other files in this folder are in context

// Boost imports
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

// Boost naming setup
namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

using namespace logging;

// Websocket handler code
void do_session(tcp::socket socket) {
    try {
	// Construct a stream with fancy moving stuff
	websocket::stream<tcp::socket> ws{std::move(socket)};

	// Set a decorator to control the Server of handshake
	ws.set_option(websocket::stream_base::decorator(
		    [](websocket::response_type& res) {
		    res.set(http::field::server, starkingdoms_version + "/server-sync-alpha");
		    }));

	ws.accept(); // Accept the websocket connection

	for (;;) {
	    beast::flat_buffer buffer; // Buffer to hold incoming message

	    ws.read(buffer); // Get message from client

	    ws.text(ws.got_text());
	    ws.write(buffer.data()); // Echo data back
	}
    } catch (beast::system_error const& se) {
	// Session was closed
	if (se.code() != websocket::error::closed) {
	    error("ConnectionHandler", se.code().message());
	}
    } catch (std::exception const& e) {
	error("ConnectionErrorHandler", e.what());
    }
    log("ConnectionHandler", "Disconnected");
}

int main(int argc, char** argv) {
    log("ServerThread", "Welcome to StarKingdoms! Version: " + starkingdoms_version);
    log("ServerThread", "Initializing websocket connections");

    try {
	auto const address = net::ip::make_address("127.0.0.1");
	auto const port = static_cast<unsigned short>(std::atoi("8443"));

	net::io_context ioctx{1};

	tcp::acceptor acceptor{ioctx, {address, port}};
	for (;;) {
	    tcp::socket socket{ioctx};

	    acceptor.accept(socket);

	    log("ConnectionHandler", "New connection");

	    std::thread(&do_session, std::move(socket)).detach();
	}
    } catch (const std::exception& e) {
	error("ServerThread", e.what());
	return EXIT_FAILURE;
    }
}
