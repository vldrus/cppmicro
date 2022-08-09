#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <Poco/Util/Application.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

#include "common.hpp"

using Poco::Util::Application;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPRequestHandler;

class Handler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res)
    {
        try
        {
            Application::instance().logger()
                .information("Handling request");
            Common::respondOKWithText(res) << "hello";
        }
        catch (...)
        {
            Application::instance().logger()
                .error("Cannot handle request: %s", Common::currentExceptionText());
            Common::respondInternalServerError(res);
        }
    }
};

#endif // HANDLER_HPP
