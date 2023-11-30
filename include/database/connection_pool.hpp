#pragma once

#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

#include <memory>
#include <queue>
#include <mutex>

class MySqlConnectionPool {
public:
    MySqlConnectionPool(std::size_t poolSize)
        : driver(get_driver_instance()) // Get the driver instance
    {
        for (std::size_t i = 0; i < poolSize; ++i) {
            connections.push(createConnection()); // Store connections in the pool
        }
    }

    std::shared_ptr<sql::Connection> getConnection() {
        std::unique_lock<std::mutex> lock(mutex);
        if (connections.empty()) {
            return nullptr; // Or handle as per your requirements
        }
        auto conn = connections.front();
        connections.pop();
        return conn;
    }

    void returnConnection(std::shared_ptr<sql::Connection> conn) {
        std::unique_lock<std::mutex> lock(mutex);
        connections.push(std::move(conn));
    }

private:
    std::shared_ptr<sql::Connection> createConnection() {
        try {
            // Create and return a new connection
            std::shared_ptr<sql::Connection> conn(
                driver->connect("tcp://127.0.0.1:3306", "root", "localsqlpass"));
            return conn;
        } catch (sql::SQLException &e) {
            // Handle exceptions: log error, throw exception, or take appropriate action
            std::cerr << "Error creating MySQL connection: " << e.what() << std::endl;
            return nullptr;
        }
    }

    sql::Driver *driver;
    std::queue<std::shared_ptr<sql::Connection>> connections;
    std::mutex mutex;
};
