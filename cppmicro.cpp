/* MIT License */

#include <cpplog.h>
#include <httplib.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace std::chrono;
using namespace nlohmann;
using namespace httplib;

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
            cpplog_error << name << " error: " << e.what();

            res.status = 500;
            res.set_content("Server Error", "text/plain");
        }

        auto finish = system_clock::now();

        cpplog_debug << name << " took " << duration_cast<milliseconds>(finish - start).count() << " ms";
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

    if (config.debug) {
        cpplog::level(cpplog::debug);
    }

    cpplog_info << "Listening with config: " << json(config).dump();

    Server server;

    server.Get("/", make_handler("index_get_handler", index_get_handler));
    server.Post("/", make_handler("index_post_handler", index_post_handler));

    server.new_task_queue = []() -> ThreadPool * {
        return new ThreadPool(config.threads);
    };

    if (!server.listen("0.0.0.0", config.port)) {
        cpplog_error << "Cannot listen at port " << config.port;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
