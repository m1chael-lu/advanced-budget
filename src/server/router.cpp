
#include "../../include/server/router.hpp"
#include <unordered_map>
#include <sstream>

std::unordered_map<std::string, std::string> parseQueryParameters(const std::string& url) {
    std::unordered_map<std::string, std::string> queryParams;
    
    // Query Parameters
    auto queryStart = url.find('?');
    if (queryStart != std::string::npos) {
        std::string queryString = url.substr(queryStart + 1);
        std::istringstream queryStream(queryString);
        std::string pair;

        while (std::getline(queryStream, pair, '&')) {
            auto delimiterPos = pair.find('=');
            std::string key = pair.substr(0, delimiterPos);
            std::string value = pair.substr(delimiterPos + 1);
            
            queryParams[key] = value;
        }
    }

    return queryParams;
}

nlohmann::json parseBodyParameters(Request& req)
{
    nlohmann::json bodyParams; 
    if (req[http::field::content_type] == "application/json") {
        try {
            bodyParams = nlohmann::json::parse(req.body());
        } catch (const nlohmann::json::parse_error& e) {
            std::cout << "JSON parse error: " << e.what() << std::endl;
        }
    }
    return bodyParams;
}


ParsedRoute extractRequestParameters(std::string path, Request& req)
{
    ParsedRoute out;
    std::string base_path = path;
    auto queryStart = path.find('?');
    if (queryStart != std::string::npos) {
        base_path = path.substr(0, queryStart);
    }

    out.base_path = base_path;
    out.query_parameters = parseQueryParameters(path);
    out.body_parameters = parseBodyParameters(req);
    std::cout << "path: " << path << std::endl;
    return out;
}

void Router::addRoute(http::verb method, const std::string& path, HandlerFunction handler)
{
        Route newRoute;
        newRoute.base_path = path;
        newRoute.handler = handler;
        routes_[method][newRoute.base_path] = newRoute;
}

bool Router::handleRequest(const Request& req, Response& res)
{
        if (req.method() == boost::beast::http::verb::options) {
            // Create an appropriate response for OPTIONS
            // auto options_response = boost::beast::http::response<boost::beast::http::string_body>{};

            // Set CORS headers
            res.set(boost::beast::http::field::access_control_allow_origin, "*");
            res.set(boost::beast::http::field::access_control_allow_methods, "GET, POST, OPTIONS");
            res.set(boost::beast::http::field::access_control_allow_headers, "Content-Type, Authorization");

            // Set other response properties as needed and send the response
            // Ensure to send a 200 OK status
            res.result(boost::beast::http::status::ok);
            return true;
        } else {
            const auto& methodRoutes = routes_.find(req.method());
            if (methodRoutes != routes_.end()) {
                std::string path(req.target().begin(), req.target().end());
                ParsedRoute parsed = extractRequestParameters(path, const_cast<Request&>(req));
                const auto& handler = methodRoutes->second.find(parsed.base_path);
                if (handler != methodRoutes->second.end()) {
                    std::shared_ptr<sql::Connection> conn = connPool_.getConnection();
                    handler->second.handler(req, res, parsed, conn);
                    connPool_.returnConnection(conn);
                    return true;
                }
            }
        }
        return false;
}
