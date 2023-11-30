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

void handleAddUser(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn)
{
    res.set(http::field::content_type, "application/json");
    std::string username = parsed.body_parameters.at("username");
    std::string password = parsed.body_parameters.at("password");
    std::string email = parsed.body_parameters.at("email");
    int id_to_return = -1;
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute("USE budgetDatabase");
        stmt->execute("INSERT INTO users (username, PasswordHash, email) VALUES ('" + username + "', '" + password + "', '" + email + "')");
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID()"));
        if (res->next()) {
            int lastId = res->getInt(1);
            std::cout << "Expense record inserted successfully with ID: " << lastId << std::endl;
            id_to_return = lastId;
        } else {
            std::cerr << "Failed to retrieve last insert ID." << std::endl;
        }

        std::cout << "Expense record inserted successfully" << std::endl;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }

    nlohmann::json response_body = {
        {"id", id_to_return}
    };
    res.body() = response_body.dump();
}

void handleAddExpense(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn)
{
    res.set(http::field::content_type, "application/json");
    int id_to_return = -1;
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());

        stmt->execute("USE budgetDatabase");
        std::string startPeriod = parsed.body_parameters.at("startPeriod");
        std::string endPeriod = parsed.body_parameters.at("endPeriod");
        if (!startPeriod.empty() && startPeriod.front() == '"' && startPeriod.back() == '"') {
            startPeriod = startPeriod.substr(1, startPeriod.length() - 2);
        }
        if (!endPeriod.empty() && endPeriod.front() == '"' && endPeriod.back() == '"') {
            endPeriod = endPeriod.substr(1, endPeriod.length() - 2);
        }
        std::stringstream sql;
        sql << "INSERT INTO Expenses (UserID, Name, Description, StartPeriod, EndPeriod, Value) VALUES (";
        sql << parsed.body_parameters.at("userid") << ", ";
        sql << "'" << parsed.body_parameters.at("name") << "', ";
        sql << "'" << parsed.body_parameters.at("description") << "', ";
        sql << "'" << startPeriod << "', "; // Ensure these are in 'YYYY-MM-DD HH:MM:SS' format
        sql << "'" << endPeriod << "', ";
        sql << parsed.body_parameters.at("value") << ")";

        stmt -> executeUpdate(sql.str());
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID()"));
        if (res->next()) {
            int lastId = res->getInt(1);
            std::cout << "Expense record inserted successfully with ID: " << lastId << std::endl;
            id_to_return = lastId;
        } else {
            std::cerr << "Failed to retrieve last insert ID." << std::endl;
        }

        std::cout << "Expense record inserted successfully" << std::endl;
    } catch(sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }

    nlohmann::json out;
    out["id"] = id_to_return;
    res.body() = out.dump();
}

void deleteExpense(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn)
{
    res.set(http::field::content_type, "application/json");
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute("USE budgetDatabase");
        stmt->execute("DELETE FROM Expenses WHERE ExpenseID = " + parsed.query_parameters.at("id"));
        std::cout << "Expense record deleted successfully" << std::endl;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
} 

void setupRoutes(Router& router) 
{
    router.addRoute(http::verb::get, "/", handleBase);
    router.addRoute(http::verb::get, "/about", handleAbout);
    router.addRoute(http::verb::get, "/apitest", handleAPITest);
    router.addRoute(http::verb::post, "/posttest", handleAPIPost);
    router.addRoute(http::verb::post, "/adduser", handleAddUser);
    router.addRoute(http::verb::post, "/addexpense", handleAddExpense);
    router.addRoute(http::verb::delete_, "/deleteexpense", deleteExpense);
}
