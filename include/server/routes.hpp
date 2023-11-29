#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "router.hpp"

namespace beast = boost::beast;         
namespace http = beast::http;           
namespace net = boost::asio;            
using tcp = net::ip::tcp;  

void handleBase(const Request& req, Response& res, ParsedRoute parsed);

void handleAbout(const Request& req, Response& res, ParsedRoute parsed);

void handleAPITest(const Request& req, Response& res, ParsedRoute parsed);

void setupRoutes(Router& router);
