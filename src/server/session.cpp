#include "../../include/server/session.hpp" 


// Session class to manage individual connections
// Constructor
Session::Session(tcp::socket&& socket, Router& router) 
    : socket_(std::forward<tcp::socket>(socket)), router_(router) {}

void Session::handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    if (!router_.handleRequest(req, res)) {
        // Handle unknown route
        res.result(http::status::not_found);
        res.set(http::field::content_type, "text/plain");
        res.body() = "Not Found";
    }
    res.prepare_payload();
}
void Session::start() {
    do_read();
}

void Session::do_read() {
        auto self = shared_from_this();
        http::async_read(socket_, buffer_, req_,
            [this, self](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    handle_request(req_, res_);
                    do_write();
                } else {
                    log("Read error: " + ec.message());
                }
            });
    }

void Session::do_write() {
    auto self = shared_from_this();
    http::async_write(socket_, res_,
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                socket_.shutdown(tcp::socket::shutdown_send, ec);
            } else {
                log("Write error: " + ec.message());
            }
        });
}
