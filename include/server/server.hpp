// server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP

#include "router.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <iostream>
#include "../log.hpp"

namespace net = boost::asio;            
using tcp = net::ip::tcp;      

class Server {
public:
    Server(net::io_context& ioc, unsigned short port, Router router);

    void run();
    void do_accept();

private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    Router router_;
};

#endif // SERVER_HPP
