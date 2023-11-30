#include "../../include/server/routes.hpp"

void handleBase(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn) 
{
    res.set(http::field::content_type, "text/html");
    res.body() = "Hello world!";
}

void handleAbout(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn) 
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

void handleAPIPost(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn)
{
    res.set(http::field::content_type, "application/json");
    res.body() = parsed.body_parameters.dump();
}

void handleAPITest(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn) 
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

void handleInsertUser(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn)
{
    res.set(http::field::content_type, "application/json");
    std::string username = parsed.body_parameters.at("username");
    std::string password = parsed.body_parameters.at("password");
    std::string email = parsed.body_parameters.at("email");

    sql::Statement* stmt = conn->createStatement();
    stmt->execute("USE budgetDatabase");
    stmt->execute("INSERT INTO users (username, PasswordHash, email) VALUES ('" + username + "', '" + password + "', '" + email + "')");

    res.body() = parsed.body_parameters.dump();
}


void setupRoutes(Router& router) 
{
    router.addRoute(http::verb::get, "/", handleBase);
    router.addRoute(http::verb::get, "/about", handleAbout);
    router.addRoute(http::verb::get, "/apitest", handleAPITest);
    router.addRoute(http::verb::post, "/posttest", handleAPIPost);
    router.addRoute(http::verb::post, "/insertuser", handleInsertUser);
}
