// lib/net/http.cc
#include "lib/net/http.h"
#include <algorithm>

namespace net {

Response Response::Ok(std::string body)           { return {200, std::move(body), {}}; }
Response Response::BadRequest(std::string body)   { return {400, std::move(body), {}}; }
Response Response::Unauthorized(std::string body) { return {401, std::move(body), {}}; }
Response Response::InternalError(std::string body){ return {500, std::move(body), {}}; }

core::Result<std::string> GetHeader(const Headers& hdrs, const std::string& key) {
    std::string lk = key;
    std::transform(lk.begin(), lk.end(), lk.begin(), ::tolower);
    for (auto& [k, v] : hdrs) {
        std::string lki = k;
        std::transform(lki.begin(), lki.end(), lki.begin(), ::tolower);
        if (lki == lk) return core::Result<std::string>::Ok(v);
    }
    return core::Result<std::string>::Err("header not found: " + key);
}

}  // namespace net
