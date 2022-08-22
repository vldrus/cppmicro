#include <string>
#include <vector>
#include <exception>
#include <cpplog.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>

using std::string;
using std::vector;
using std::exception;
using cpplog::log;
using httplib::Server;
using httplib::Request;
using httplib::Response;
using httplib::ThreadPool;
using Handler = httplib::Server::Handler;
using json = nlohmann::json;

static const int PORT = 8080;
static const int THREADS = 1000;

struct User {
    string username;
    string password;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, password)
};

Handler make_handler(string name, Handler handler);

void get_all_users(const Request& req, Response& res);

int main()
{
    log::level = cpplog::DEBUG;

    LOG_INFO << "Listening at port " << PORT << " with " << THREADS << " threads";

    Server server;

    server.Get("/users", make_handler("get_all_users", get_all_users));

    server.new_task_queue = [] {
        return new ThreadPool(THREADS);
    };

    server.listen("0.0.0.0", PORT);
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
