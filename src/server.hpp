#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <Poco/Util/Application.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPServer.h>

#include "router.hpp"

using std::string;
using std::vector;
using Poco::Util::Application;
using Poco::Util::ServerApplication;
using Poco::Net::HTTPServer;

class Server : public ServerApplication
{
protected:
    int main(const vector<string>& args)
    {
        Application::instance().logger()
            .information("Listening at port %d", PORT);

        HTTPServer server(new Router(), PORT);
        server.start();

        waitForTerminationRequest();
        server.stop();

        return Application::EXIT_OK;
    }

private:
    static const int PORT = 8080;
};

#endif // SERVER_HPP
