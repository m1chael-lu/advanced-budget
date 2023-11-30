#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "router.hpp"
#include "../database/connection_pool.hpp"

namespace beast = boost::beast;         
namespace http = beast::http;           
namespace net = boost::asio;            
using tcp = net::ip::tcp;  

void handleBase(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn);

void handleAbout(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn);

void handleAPITest(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn);

void handleAPIPost(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn);

void handleAddUser(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn);

void handleAddExpense(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn);

void setupRoutes(Router& router);
