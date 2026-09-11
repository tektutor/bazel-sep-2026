#pragma once
// services/auth/auth_service.h
#include "lib/core/result.h"
#include "lib/net/http.h"
#include <string>

namespace auth {

class AuthService {
public:
    explicit AuthService(std::string secret);

    core::Result<std::string> Login(const std::string& username,
                                    const std::string& password) const;

    core::Result<std::string> Validate(const std::string& token) const;

    net::Response HandleLogin(const net::Request& req) const;

private:
    std::string secret_;
};

}  // namespace auth
