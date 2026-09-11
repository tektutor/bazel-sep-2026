// services/gateway/gateway.cc
#include "services/gateway/gateway.h"
#include "lib/core/logger.h"

namespace gateway {

namespace { core::Logger kLog("gateway"); }

void Gateway::Register(const std::string& method, const std::string& path,
                        Handler handler, bool auth_required) {
    std::string key = method + " " + path;
    routes_[key] = {std::move(handler), auth_required};
    kLog.Info("registered route: " + key);
}

net::Response Gateway::Dispatch(const net::Request& req) const {
    std::string key = req.method + " " + req.path;
    auto it = routes_.find(key);
    if (it == routes_.end()) return {404, "not found", {}};

    const Route& route = it->second;
    if (route.auth_required) {
        if (!auth_) return net::Response::InternalError("auth service unavailable");
        auto bearer = net::GetHeader(req.headers, "Authorization");
        if (bearer.err()) return net::Response::Unauthorized("missing Authorization header");
        std::string hdr = bearer.value();
        const std::string prefix = "Bearer ";
        if (hdr.substr(0, prefix.size()) != prefix)
            return net::Response::Unauthorized("expected Bearer token");
        auto user = auth_->Validate(hdr.substr(prefix.size()));
        if (user.err()) return net::Response::Unauthorized(user.error());
    }
    return route.handler(req);
}

}  // namespace gateway
