#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "common.hpp"
#include "handler.hpp"

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;

class Router : public HTTPRequestHandlerFactory
{
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& req)
    {
        if (req.getURI() == "/")
        {
            return new Handler();
        }

        return new NotFoundHandler();
    }

private:
    class NotFoundHandler : public HTTPRequestHandler
    {
    public:
        void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res)
        {
            Common::respondNotFound(res);
        }
    };
};

#endif // ROUTER_HPP
