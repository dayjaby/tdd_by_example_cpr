#ifndef CPR_CPR_H
#define CPR_CPR_H

#include <thread>
#include <map>
#include <curl/curl.h>

#include "cookies.h"

namespace cpr {

class Url {
public:
    Url(const std::string& url) : url_(url) {
    }

    bool operator==(const Url& b) const {
        return url_ == b.url_;
    }

    std::string url_;
};

enum class ErrorCode {
    OK
};

struct Error {
    ErrorCode code;
};

struct Response {
    std::string text;
    Url url;
    std::map<std::string, std::string> header;
    uint16_t status_code;
    Error error;

    std::string __full_header;
};

} // namespace cpr

#endif // CPR_CPR_H
