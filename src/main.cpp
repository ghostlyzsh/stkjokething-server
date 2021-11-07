#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "box2d.h"

using tcp = boost::asio::ip::tcp;

int main(int argc, char** argv) {
    const auto address = boost::asio::ip::make_address("127.0.0.1");
    const auto port = static_cast<unsigned short>(std::atoi("8080"));

    boost::asio::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};

    while(1){
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::cout<<"socket accepted"<<std::endl;
    }
    
    return 0;
}
