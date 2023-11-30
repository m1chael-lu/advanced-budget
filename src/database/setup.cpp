#include <iostream>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

int main() {
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;

        // Create a connection
        std::cout << "Connecting to database..." << std::endl;
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "localsqlpass");
        std::cout << "Connected!" << std::endl;

        // Create a new database if it doesn't exist
        stmt = con->createStatement();
        stmt->execute("CREATE DATABASE IF NOT EXISTS budgetDatabase");
        std::cout << "Database created or already exists." << std::endl;

        // Use the new database
        stmt->execute("USE budgetDatabase");

        // Create the 'Users' table
        stmt->execute("CREATE TABLE IF NOT EXISTS Users ("
                      "UserID INT AUTO_INCREMENT PRIMARY KEY, "
                      "Username VARCHAR(255) NOT NULL, "
                      "Email VARCHAR(255) NOT NULL, "
                      "PasswordHash VARCHAR(255) NOT NULL)");

        // Create the 'Expenses' table
        stmt->execute("CREATE TABLE IF NOT EXISTS Expenses ("
                      "ExpenseID INT AUTO_INCREMENT PRIMARY KEY, "
                      "UserID INT, "
                      "Name VARCHAR(255), "
                      "Description TEXT, "
                      "StartPeriod DATETIME, "
                      "EndPeriod DATETIME, "
                      "Value FLOAT, "
                      "FOREIGN KEY (UserID) REFERENCES Users(UserID))");

        std::cout << "Tables created or already exist." << std::endl;

        // Clean up
        delete stmt;
        delete con;

    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return 0;
}
