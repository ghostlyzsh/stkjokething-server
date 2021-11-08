//
// Created by c0repwn3r on 11/8/21
//

// libstdc++ imports
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// Boost imports
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

// Boost namespace setup
namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

#include "starkingdoms.hpp" // Include gluefile

using namespace logging;

// Error reporting
void fail(beast::error_code ec, std::string what) {
    error(what, ec.message());
}

// Echo server
class session : public std::enable_shared_from_this<session> {
    websocket::stream<beast::tcp_stream> ws_; // setup bare websocket stream
    beast::flat_buffer buffer_; // setup bare data buf

    public:
    // take over socket ownership
    explicit session(tcp::socket &&socket) : ws_(std::move(socket)) {}

    // make sure we are on the correct executor
    void run() {
	// Need to be on a strand to do async stuff on IO stuff.
	net::dispatch(ws_.get_executor(),
		beast::bind_front_handler(
		    &session::on_run,
		    shared_from_this()));
    }

    // start async stuff
    void on_run() {
	// Set timeout settings
	ws_.set_option(
		websocket::stream_base::timeout::suggested(
		    beast::role_type::server));

	// set decorator to change Server of handshake
	ws_.set_option(websocket::stream_base::decorator(
		    [](websocket::response_type &res) {
		    res.set(http::field::server, starkingdoms_version + "/starkingdoms");
		    }));

	// accept ws upgrade handshake
	ws_.async_accept(
		beast::bind_front_handler(
		    &session::on_accept,
		    shared_from_this()));
    }

    void on_accept(beast::error_code ec) {
	if (ec) {
	    return fail(ec, "accept");
	}

	do_read();
    }

    void do_read() {
	ws_.async_read(buffer_,
		beast::bind_front_handler(
		    &session::on_read,
		    shared_from_this()));
    }


    void on_read(beast::error_code ec, std::size_t bytes_transferred) {
	boost::ignore_unused(bytes_transferred);

	if (ec == websocket::error::closed) {
	    // socket connection closed
	    return;
	}

	if (ec) {
	    fail(ec, "msg-read");
	}

	// ---===--- MESSAGE HANDLER ---===---
	// EDIT THIS IF YOU WANT THIS TO DO SOMETHING DIFFERENT
	ws_.text(ws_.got_text()); // set message type to text

	ws_.async_write(
		buffer_.data(),
		beast::bind_front_handler(
		    &session::on_write,
		    shared_from_this()));
    }
	// --===--- MESSAGE HANDLER --==--

	void on_write(beast::error_code ec, std::size_t bytes_transferred) {
	    boost::ignore_unused(bytes_transferred);

	    if (ec) {
		return fail(ec, "write");
	    }

	    buffer_.consume(buffer_.size());

	    // ---===--- REMOVE THIS LINE IF YOU DONT WANT INFINITE READLOOP
	    do_read();
	}
    
};

// Accepts incoming connections and launches sessions
class listener : public std::enable_shared_from_this<listener> {
    net::io_context &ioc_;
    tcp::acceptor acceptor_;

    public:
    listener(net::io_context &ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(ioc) {
	beast::error_code ec;

	// open acceptor
	acceptor_.open(endpoint.protocol(), ec);
	if (ec) {
	    fail(ec, "open");
	    return;
	}

	// AAR
	acceptor_.set_option(net::socket_base::reuse_address(true), ec);
	if (ec) {
	    fail(ec, "set aar");
	    return;
	}

	// address bind
	acceptor_.bind(endpoint, ec);
	if (ec) {
	    fail(ec, "address-bind");
	    return;
	}

	// start-lfa
	acceptor_.listen(
		net::socket_base::max_listen_connections, ec);
	if (ec) {
	    fail(ec, "start lfa");
	    return;
	}
    }

    void run() {
	do_accept();
    }

    private:
    void do_accept() {
	// new conn gets its own strand
	acceptor_.async_accept(
		net::make_strand(ioc_),
		beast::bind_front_handler(
		    &listener::on_accept,
		    shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket) {
	if (ec) {
	    fail(ec, "conn accept");
	} else {
	    std::make_shared<session>(std::move(socket))->run();
	}

	do_accept();
    }
};

int main(int argc, char* argv[]) {
    auto const address = net::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(std::atoi("8443"));
    auto const threads = std::max<int>(1, std::atoi("10"));

    net::io_context ioc{threads};

    std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
	v.emplace_back(
		[&ioc]
		{
		ioc.run();
		});
    ioc.run();

    return EXIT_SUCCESS;
}
