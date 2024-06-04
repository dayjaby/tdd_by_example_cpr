#ifndef CPR_SESSION_H
#define CPR_SESSION_H

#include "cpr.h"

#include <sstream>
#include <algorithm>
#include <cctype>

namespace cpr {

int writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
    if(writerData == nullptr)
        return 0;

    writerData->append(data, size*nmemb);
    return size * nmemb;
}

void parse_header(const std::string& full_header, std::map<std::string, std::string>& header) {
    std::istringstream iss(full_header);
    for (std::string line; std::getline(iss, line); ) {
        std::size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::transform(line.begin(), line.begin() + pos, line.begin(),
                [](unsigned char c){ return std::tolower(c); }
            );
            std::size_t rpos = line.find('\r');
            header[line.substr(0, pos)] = line.substr(pos+2, rpos == std::string::npos ? std::string::npos : line.length() - 3 - pos);
        }
    }
}

bool prepare_get_request(CURL* curl, Response& response) {
    CURLcode res;
    res = curl_easy_setopt(curl, CURLOPT_URL, response.url.url_.c_str());
    if (res != CURLE_OK) {
        return false;
    }
    res = curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    if (res != CURLE_OK) {
        return false;
    }
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    if (res != CURLE_OK) {
        return false;
    }
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.text);
    if (res != CURLE_OK) {
        return false;
    }
    std::string header_string;
    res = curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.__full_header);
    if (res != CURLE_OK) {
        return false;
    }
    return true;
}

Response Get(const Url& url) {
    Response response{{}, url, {}, 0, ErrorCode::OK, {}};
    CURL* curl = curl_easy_init();
    if (curl) {
        bool ok = prepare_get_request(curl, response);
        if (ok) {
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                parse_header(response.__full_header, response.header);
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status_code);
            }
        }
        curl_easy_cleanup(curl);
        curl = nullptr;
    }
    return response;
}

} // namespace cpr

#endif // CPR_SESSION_H
