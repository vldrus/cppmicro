#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <cpplog.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>

using std::string;
using std::vector;
using std::ifstream;
using std::exception;
using cpplog::log;
using cpplog::loglevel;
using httplib::Server;
using httplib::Request;
using httplib::Response;
using httplib::ThreadPool;
using Handler = httplib::Server::Handler;
using json = nlohmann::json;

struct Config {
    loglevel log_level;
    size_t threads;
    int port;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, log_level, threads, port)
};

struct User {
    string username;
    string password;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, password)
};

Handler make_handler(string name, Handler handler);

void get_all_users(const Request& req, Response& res);

int main()
{
    Config config = json::parse(ifstream("config.json"));

    log::level = config.log_level;

    LOG_INFO << "Listening at port " << config.port << " with " << config.threads << " threads";

    Server server;

    server.Get("/users", make_handler("get_all_users", get_all_users));

    server.new_task_queue = [=] {
        return new ThreadPool(config.threads);
    };

    server.listen("0.0.0.0", config.port);
}

Handler make_handler(string name, Handler handler)
{
    auto error = [=](string what, Response& res) {
        LOG_ERROR << name << " error: " << what;

        res.status = 500;
        res.set_content("Server Error", "text/plain");
    };

    return [=](const Request& req, Response& res) {
        LOG_DEBUG << name << " start";

        try {
            handler(req, res);
        } catch (const exception& e) {
            error(e.what(), res);
        } catch (...) {
            error("unknown", res);
        }

        LOG_DEBUG << name << " finish";
    };
}

void get_all_users(const Request& req, Response& res)
{
    vector<User> users;

    users.push_back(User{"user1", "pass1"});
    users.push_back(User{"user2", "pass2"});

    LOG_DEBUG << "get_all_users found: " << users.size();

    res.set_content(json(users).dump(), "application/json");
}
