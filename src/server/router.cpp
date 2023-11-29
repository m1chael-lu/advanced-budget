
#include "../../include/server/router.hpp"
#include <unordered_map>
#include <sstream>

std::unordered_map<std::string, std::string> parseQueryParameters(const std::string& url) {
    std::unordered_map<std::string, std::string> queryParams;
    
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

ParsedRoute parseRawRoute(std::string path)
{
    ParsedRoute out;
    std::string base_path = path;
    auto queryStart = path.find('?');
    if (queryStart != std::string::npos) {
        base_path = path.substr(0, queryStart);
    }

    out.base_path = base_path;
    out.query_parameters = parseQueryParameters(path);
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
        const auto& methodRoutes = routes_.find(req.method());
        if (methodRoutes != routes_.end()) {
            std::string path(req.target().begin(), req.target().end());
            ParsedRoute parsed = parseRawRoute(path);
            
            const auto& handler = methodRoutes->second.find(parsed.base_path);
            if (handler != methodRoutes->second.end()) {
                handler->second.handler(req, res, parsed);
                return true;
            }
        }
        return false;
}
