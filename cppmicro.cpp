/* MIT License */

#include <cpplog.h>
#include <httplib.h>
#include <nlohmann/json.hpp>

using std::string;
using std::ifstream;
using std::exception;
using nlohmann::json;
using httplib::Server;
using httplib::Request;
using httplib::Response;
using httplib::ThreadPool;

struct Config {
    cpplog::loglevel loglevel;
    size_t threads;
    int port;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, loglevel, threads, port)
};

struct User {
    string username;
    string password;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, password)
};

auto make_handler(const string &name, const Server::Handler &handler) -> Server::Handler {
    return [name, handler](const Request &req, Response &res) -> void {
        LOG_DEBUG << name << " start";

        try {
            handler(req, res);
        } catch (const exception &e) {
            LOG_ERROR << name << " error: " << e.what();

            res.status = 500;
            res.set_content("Server Error", "text/plain");
        }

        LOG_DEBUG << name << " finish";
    };
}

auto index_get_handler(const Request &req, Response &res) -> void {
    res.set_content("Hello!", "text/plain");
}

auto index_post_handler(const Request &req, Response &res) -> void {
    User user;

    try {
        user = json::parse(req.body);
    } catch (const exception &e) {
        res.status = 400;
        return;
    }

    res.set_content(json(user).dump(), "application/json");
}

auto main() -> int {
    Config config = json::parse(ifstream("config.json"));

    cpplog::level(config.loglevel);

    LOG_INFO << "Listening at port " << config.port << " with " << config.threads << " threads";

    Server server;

    server.Get("/", make_handler("index_get_handler", index_get_handler));
    server.Post("/", make_handler("index_post_handler", index_post_handler));

    server.new_task_queue = [config]() -> ThreadPool * {
        return new ThreadPool(config.threads);
    };

    if (!server.listen("0.0.0.0", config.port)) {
        LOG_ERROR << "Cannot listen at port " << config.port;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
