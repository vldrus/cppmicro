#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <Poco/Exception.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::ostream;
using std::string;
using std::exception;
using std::current_exception;
using std::rethrow_exception;
using Poco::Exception;
using Poco::Net::HTTPServerResponse;

using HTTPStatus = HTTPServerResponse::HTTPStatus;

class Common
{
public:
    static string currentExceptionText()
    {
        if (current_exception())
        {
            try
            {
                rethrow_exception(current_exception());
            }
            catch (const Exception& e)
            {
                return e.displayText();
            }
            catch (const exception& e)
            {
                return e.what();
            }
            catch (...)
            {
            }
        }

        return "Unknown";
    }

    static ostream& respondOKWithText(HTTPServerResponse& res)
    {
        return respond(HTTPStatus::HTTP_OK, TEXT_PLAIN, res);
    }

    static void respondNotFound(HTTPServerResponse& res)
    {
        respondError(HTTPStatus::HTTP_NOT_FOUND, res);
    }

    static void respondInternalServerError(HTTPServerResponse& res)
    {
        respondError(HTTPStatus::HTTP_INTERNAL_SERVER_ERROR, res);
    }

    static void respondError(HTTPStatus status, HTTPServerResponse& res)
    {
        respond(status, TEXT_PLAIN, res) << res.getStatus() << " " << res.getReason();
    }

    static ostream& respond(HTTPStatus status, string contentType, HTTPServerResponse& res)
    {
        res.setStatusAndReason(status);
        res.setContentType(contentType);

        return res.send();
    }

private:
    inline static const string TEXT_PLAIN = "text/plain;charset=utf-8";
};

#endif // COMMON_HPP
