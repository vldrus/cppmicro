#include <cpplog.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>

struct User {
    std::string username;
    std::string password;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, password)
};

auto make_handler(const std::string &name, const httplib::Server::Handler &handler) -> httplib::Server::Handler {
    return [name, handler](const httplib::Request &req, httplib::Response &res) -> void {
        LOG_DEBUG << name << " start";

        try {
            handler(req, res);
        } catch (const std::exception &e) {
            LOG_ERROR << name << " error: " << e.what();

            res.status = 500;
            res.set_content("Server Error", "text/plain");
        }

        LOG_DEBUG << name << " finish";
    };
}

auto index_get_handler(const httplib::Request &req, httplib::Response &res) -> void {
    res.set_content("Hello!", "text/plain");
}

auto index_post_handler(const httplib::Request &req, httplib::Response &res) -> void {
    User user;

    try {
        user = nlohmann::json::parse(req.body);
    } catch (const std::exception &e) {
        res.status = 400;
        return;
    }

    res.set_content(nlohmann::json(user).dump(), "application/json");
}

auto main() -> int {
    nlohmann::json config = nlohmann::json::parse(std::ifstream("config.json"));

    int loglevel = config["loglevel"].get<int>();
    int port = config["port"].get<int>();

    cpplog::level = static_cast<cpplog::loglevel>(loglevel);

    LOG_INFO << "Listening at port " << port;

    httplib::Server server;

    server.Get("/", make_handler("index_get_handler", index_get_handler));
    server.Post("/", make_handler("index_post_handler", index_post_handler));

    if (!server.listen("0.0.0.0", port)) {
        LOG_ERROR << "Cannot listen at port " << port;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
