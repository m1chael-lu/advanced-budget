#include "../include/server/server.hpp"
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "../include/log.hpp"
#include <csignal>
#include "../include/server/router.hpp"
#include "../include/server/routes.hpp"

namespace beast = boost::beast;         
namespace http = beast::http;           
namespace net = boost::asio;            
using tcp = net::ip::tcp;      

void log(const std::string& message) {
    std::cerr << message << std::endl;
}

// Signal handler for graceful shutdown
std::atomic<bool> shutdown_flag(false);

void signalHandler(int signum) {
    log("Interrupt signal (" + std::to_string(signum) + ") received.");
    shutdown_flag = true;
}

int main() {
    try {
        net::io_context ioc;
        net::io_context::work idleWork(ioc);
        unsigned short port = 8080;
        Router router;
        setupRoutes(router);

        Server server{ioc, port, router};

        // Setup signal handler for graceful shutdown
        signal(SIGINT, signalHandler);

        // Thread pool
        std::vector<std::thread> pool;
        for (std::size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            pool.emplace_back([&ioc] { ioc.run(); });
        }
        server.run();

        std::cout << "Server is running. Press CTRL+C to stop.\n";

        // Wait for signal
        while (!shutdown_flag) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Stopping server
        ioc.stop();
        for (auto& t : pool) {
            if (t.joinable()) {
                t.join();
            }
        }

        std::cout << "Server stopped.\n";
    } catch (std::exception& e) {
        log("Server error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}
