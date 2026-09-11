// services/auth/auth_service.cc
#include "services/auth/auth_service.h"
#include "lib/crypto/crypto.h"
#include "lib/core/logger.h"
#include <sstream>

namespace auth {

namespace { core::Logger kLog("auth"); }

AuthService::AuthService(std::string secret) : secret_(std::move(secret)) {}

core::Result<std::string> AuthService::Login(
        const std::string& username, const std::string& password) const {
    if (username.empty() || password.empty())
        return core::Result<std::string>::Err("username and password required");
    auto hash = crypto::Sha256Hex(username + ":" + password + ":" + secret_);
    if (hash.err()) return core::Result<std::string>::Err(hash.error());
    std::string token = crypto::Base64Encode(username + "." + hash.value());
    kLog.Info("login ok: " + username);
    return core::Result<std::string>::Ok(token);
}

core::Result<std::string> AuthService::Validate(const std::string& token) const {
    auto decoded = crypto::Base64Decode(token);
    if (decoded.err()) return core::Result<std::string>::Err("invalid token encoding");
    auto dot = decoded.value().find('.');
    if (dot == std::string::npos) return core::Result<std::string>::Err("malformed token");
    return core::Result<std::string>::Ok(decoded.value().substr(0, dot));
}

net::Response AuthService::HandleLogin(const net::Request& req) const {
    std::string user, pass;
    std::istringstream ss(req.body);
    std::string kv;
    while (std::getline(ss, kv, '&')) {
        auto eq = kv.find('=');
        if (eq == std::string::npos) continue;
        auto k = kv.substr(0, eq), v = kv.substr(eq + 1);
        if (k == "username") user = v;
        if (k == "password") pass = v;
    }
    auto result = Login(user, pass);
    if (result.err()) return net::Response::Unauthorized(result.error());
    return net::Response::Ok(result.value());
}

}  // namespace auth
