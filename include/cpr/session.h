#ifndef CPR_SESSION_H
#define CPR_SESSION_H

#include "cpr.h"

namespace cpr {

Response Get(const Url& url) {
    Response response{{}, url, {}, 0, ErrorCode::OK};
    return response;
}

} // namespace cpr

#endif // CPR_SESSION_H
