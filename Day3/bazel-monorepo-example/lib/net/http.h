#pragma once
// lib/net/http.h
#include <string>
#include <unordered_map>
#include "lib/core/result.h"

namespace net {

using Headers = std::unordered_map<std::string, std::string>;

struct Request {
    std::string method;
    std::string path;
    std::string body;
    Headers     headers;
};

struct Response {
    int         status = 200;
    std::string body;
    Headers     headers;

    static Response Ok(std::string body);
    static Response BadRequest(std::string body);
    static Response Unauthorized(std::string body);
    static Response InternalError(std::string body);
};

core::Result<std::string> GetHeader(const Headers& hdrs, const std::string& key);

}  // namespace net
