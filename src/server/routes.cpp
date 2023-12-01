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
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::access_control_allow_methods, "GET, POST, OPTIONS");
    res.set(boost::beast::http::field::access_control_allow_headers, "Content-Type, Authorization");
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

void aggregateExpenses(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn)
{
    res.set(http::field::content_type, "application/json");
    nlohmann::json result;
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute("USE budgetDatabase");
        
        std::string startDate = "2023-11-15";
        std::string endDate = "2023-12-15";
        // Construct the SQL query with recursive CTE
        std::string query = R"(
            WITH RECURSIVE DateSeries AS (
                SELECT CAST(')" + startDate + R"(' AS DATE) AS Date
                UNION ALL
                SELECT Date + INTERVAL 1 DAY FROM DateSeries
                WHERE Date < CAST(')" + endDate + R"(' AS DATE)
            )
            SELECT
                DateSeries.Date,
                SUM(
                    CASE 
                        WHEN DATE(Expenses.EndPeriod) > DATE(Expenses.StartPeriod) THEN 
                            Expenses.Value / (DATEDIFF(DATE(Expenses.EndPeriod), DATE(Expenses.StartPeriod)) + 1)
                        ELSE Expenses.Value 
                    END
                ) AS DailyValue
            FROM
                DateSeries
            LEFT JOIN
                Expenses ON DateSeries.Date BETWEEN DATE(Expenses.StartPeriod) AND DATE(Expenses.EndPeriod) AND Expenses.UserID = 2
            GROUP BY
                DateSeries.Date
        )";
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        
        nlohmann::json data = nlohmann::json::array();
        while (res->next()) {
            std::string date = res->getString("Date");
            double dailyValue = res->getDouble("DailyValue");
            data.push_back({{"day", date}, {"aggregate_value", dailyValue}});
        }
        result["data"] = data;
    } catch(sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        result["error"] = "Database error";
    }
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::access_control_allow_methods, "GET, POST, OPTIONS");
    res.set(boost::beast::http::field::access_control_allow_headers, "Content-Type, Authorization");
    res.body() = result.dump();
}

void retrieveUserExpenses(const Request& req, Response& res, ParsedRoute parsed, std::shared_ptr<sql::Connection> conn)
{
    res.set(http::field::content_type, "application/json");
    nlohmann::json result;
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute("USE budgetDatabase");
        std::unique_ptr<sql::ResultSet> results(stmt->executeQuery("SELECT * FROM Expenses WHERE UserID = " + parsed.query_parameters.at("id")));
        
        nlohmann::json data = nlohmann::json::array();
        while (results->next()) {
            int id = results->getInt("ExpenseID");
            int user_id = results->getInt("UserID");
            std::string name = results->getString("Name");
            std::string description = results->getString("Description");
            std::string startPeriod = results->getString("StartPeriod");
            std::string endPeriod = results->getString("EndPeriod");
            double value = results->getDouble("Value");
            data.push_back({{"id", id}, {"user_id", user_id}, {"name", name}, {"description", description}, {"start_period", startPeriod}, {"end_period", endPeriod}, {"value", value}});
        }
        result["data"] = data;
    } catch(sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        result["error"] = "Database error";
    }
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::access_control_allow_methods, "GET, POST, OPTIONS");
    res.set(boost::beast::http::field::access_control_allow_headers, "Content-Type, Authorization");
    res.body() = result.dump();
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
    router.addRoute(http::verb::get, "/aggregateexpenses", aggregateExpenses);
    router.addRoute(http::verb::get, "/retrieveuserexpenses", retrieveUserExpenses);
}
