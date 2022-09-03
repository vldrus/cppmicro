#include <cpplog.hpp>
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
    json config = json::parse(ifstream("config.json"));

    config["loglevel"].get_to(cpplog::level);

    int port;
    config["port"].get_to(port);

    int threads;
    config["threads"].get_to(threads);

    LOG_INFO << "Listening at port " << port << " with " << threads << " threads";

    Server server;

    server.Get("/", make_handler("index_get_handler", index_get_handler));
    server.Post("/", make_handler("index_post_handler", index_post_handler));

    server.new_task_queue = [threads]() -> ThreadPool * {
        return new ThreadPool(threads);
    };

    if (!server.listen("0.0.0.0", port)) {
        LOG_ERROR << "Cannot listen at port " << port;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
