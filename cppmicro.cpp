/* MIT License */

#include <cpplog.h>
#include <httplib.h>
#include <nlohmann/json.hpp>

using std::string;
using std::ifstream;
using std::exception;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using nlohmann::json;
using httplib::Server;
using httplib::Request;
using httplib::Response;
using httplib::ThreadPool;

struct Config {
    bool debug;
    int threads;
    int port;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, debug, threads, port)
};

struct User {
    string username;
    string password;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, password)
};

auto config = Config();

auto make_handler(const string &name, const Server::Handler &handler) -> Server::Handler {
    return [name, handler](const Request &req, Response &res) -> void {
        auto start = system_clock::now();

        try {
            handler(req, res);
        } catch (const exception &e) {
            cpplog() << name << " error: " << e.what();

            res.status = 500;
            res.set_content("Server Error", "text/plain");
        }

        auto finish = system_clock::now();

        if (config.debug) {
            cpplog() << name << " took " << duration_cast<milliseconds>(finish - start).count() << " ms";
        }
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
    config = json::parse(ifstream("config.json"));

    cpplog() << "Listening with config: " << json(config).dump();

    Server server;

    server.Get("/", make_handler("index_get_handler", index_get_handler));
    server.Post("/", make_handler("index_post_handler", index_post_handler));

    server.new_task_queue = []() -> ThreadPool * {
        return new ThreadPool(config.threads);
    };

    if (!server.listen("0.0.0.0", config.port)) {
        cpplog() << "Cannot listen at port " << config.port;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
