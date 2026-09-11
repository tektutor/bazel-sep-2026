#pragma once
// services/gateway/gateway.h
#include "lib/net/http.h"
#include "services/auth/auth_service.h"
#include <functional>
#include <unordered_map>
#include <string>

namespace gateway {

using Handler = std::function<net::Response(const net::Request&)>;

class Gateway {
public:
    void Register(const std::string& method,
                  const std::string& path,
                  Handler handler,
                  bool auth_required = false);

    net::Response Dispatch(const net::Request& req) const;

    void SetAuthService(const auth::AuthService* svc) { auth_ = svc; }

private:
    struct Route { Handler handler; bool auth_required; };
    std::unordered_map<std::string, Route> routes_;
    const auth::AuthService* auth_ = nullptr;
};

}  // namespace gateway
