#pragma once

#include <functional>
#include <unordered_map>
#include <string>
#include <boost/beast/http.hpp>
#include <iostream>


namespace http = boost::beast::http;

struct ParsedRoute {
    std::string base_path;
    std::unordered_map<std::string, std::string> path_parameters;
    std::unordered_map<std::string, std::string> query_parameters;
    std::unordered_map<std::string, std::string> body_parameters;
};

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;
using HandlerFunction = std::function<void(const Request&, Response&, ParsedRoute)>;

struct Route {
    std::string base_path;
    HandlerFunction handler;
};


ParsedRoute parseRawRoute(std::string path);

class Router {
public:
    void addRoute(http::verb method, const std::string& path, HandlerFunction handler);

    bool handleRequest(const Request& req, Response& res);

private:
    std::unordered_map<http::verb, std::unordered_map<std::string, Route>> routes_;
};
