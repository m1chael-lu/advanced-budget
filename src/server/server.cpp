#include "../../include/server/server.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <thread>
#include <vector>
#include "../../include/server/session.hpp"


namespace beast = boost::beast;         
namespace http = beast::http;           
namespace net = boost::asio;            
using tcp = net::ip::tcp;     

// Implement the Server constructor
Server::Server(net::io_context& ioc, unsigned short port, Router& router)
    : ioc_(ioc), acceptor_(ioc, {tcp::v4(), port}), router_(router) {}

// Implement the run method
void Server::run() {
    do_accept();
}

// Implement the do_accept method
void Server::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                // Assuming Session is defined somewhere else and has a start method
                std::make_shared<Session>(std::forward<tcp::socket>(socket), router_)->start();
            } else {
                log("Accept error: " + ec.message());
            }
            do_accept();
        });
}

    

