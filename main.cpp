#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <cpplog.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>

struct Config {
    int log;
    int port;
    int threads;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, log, port, threads)
};

struct User {
    std::string username;
    std::string password;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, password)
};

void get_all_users(const httplib::Request &req, httplib::Response &res) {
    LOG_DEBUG << "get_all_users: start";

    try {
        std::vector<User> users;

        users.push_back(User{"user1", "pass1"});
        users.push_back(User{"user2", "pass2"});

        LOG_DEBUG << "get_all_users: " << nlohmann::json(users).dump();

        res.set_content(nlohmann::json(users).dump(), "application/json");
    }
    catch (const std::exception &e) {
        LOG_ERROR << "get_all_users: " << e.what();
        res.status = 500;
    }

    LOG_DEBUG << "get_all_users: finish";
}

int main() {
    Config config = nlohmann::json::parse(std::ifstream("config.json"));

    cpplog::log::level = static_cast<cpplog::loglevel>(config.log);

    LOG_INFO << "Listening at port " << config.port;

    httplib::Server server;

    server.Get("/", get_all_users);

    server.new_task_queue = [&] {
        return new httplib::ThreadPool(config.threads);
    };

    if (!server.listen("0.0.0.0", config.port)) {
        LOG_ERROR << "Cannot listen at port " << config.port;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
