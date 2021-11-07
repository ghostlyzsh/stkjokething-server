#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "box2d.h"

#include "starkingdoms.hpp"

using tcp = boost::asio::ip::tcp;

using namespace logging;

int main(int argc, char** argv) {
   log("ServerThread", "Welcome to StarKingdoms! Version: " + starkingdoms_version);
}
