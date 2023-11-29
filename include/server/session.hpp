// Session.hpp
#ifndef SESSION_HPP
#define SESSION_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <thread>
#include <vector>
#include "../log.hpp"
#include "router.hpp"


namespace beast = boost::beast;         
namespace http = beast::http;                     
namespace net = boost::asio;            
using tcp = net::ip::tcp;    
// #include <boost/asio.hpp>
// #include <boost/beast.hpp>
// #include <memory>

// class Session : public std::enable_shared_from_this<Session> {
// public:
//     explicit Session(boost::asio::ip::tcp::socket socket);
//     void start();
//     // ...
// private:
//     boost::asio::ip::tcp::socket socket_;
//     // ...
// };

// Session class to manage individual connections
class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket&& socket, Router& router);
    void start();

// Function to handle HTTP GET requests
void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res);
private:
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    http::response<http::string_body> res_;
    Router& router_;

    void do_read();

    void do_write();
};
#endif // SESSION_HPP
