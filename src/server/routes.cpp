#include "../../include/server/routes.hpp"

void handleBase(const Request& req, Response& res, ParsedRoute parsed) 
{
    res.set(http::field::content_type, "text/html");
    res.body() = "Hello world!";
}

void handleAbout(const Request& req, Response& res, ParsedRoute parsed) 
{
    res.set(http::field::content_type, "text/html");

    std::string name;
    if (parsed.query_parameters.find("name") != parsed.query_parameters.end()) {
        name = parsed.query_parameters.at("name");
    } else {
        name = "?";
    }
    res.body() = "About " + name;
}

void handleAPITest(const Request& req, Response& res, ParsedRoute parsed) 
{
    res.set(http::field::content_type, "text/html");
    std::string one;
    std::string two;
    if (parsed.query_parameters.find("one") != parsed.query_parameters.end()) {
        one = parsed.query_parameters.at("one");
        if (parsed.query_parameters.find("two") != parsed.query_parameters.end()) {
            two = parsed.query_parameters.at("two");
        } 
    } else {
        one = "?";
    }
    res.body() = "API Test one: " + one + " two: " + two;
}

void setupRoutes(Router& router) 
{
    router.addRoute(http::verb::get, "/", handleBase);
    router.addRoute(http::verb::get, "/about", handleAbout);
    router.addRoute(http::verb::get, "/apitest", handleAPITest);
}
