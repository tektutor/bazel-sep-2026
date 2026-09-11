// app/cli/main.cc
#include "app/cli/version.h"
#include "services/auth/auth_service.h"
#include "services/gateway/gateway.h"
#include "lib/core/logger.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    core::Logger log("cli");
    log.Info(std::string("monorepo-cli version=") + version::GetVersion()
             + " build=" + version::GetBuildLabel());

    auth::AuthService auth_svc("super-secret-key");
    gateway::Gateway  gw;
    gw.SetAuthService(&auth_svc);

    gw.Register("GET", "/health", [](const net::Request&) {
        return net::Response::Ok("healthy\n");
    });

    gw.Register("POST", "/login", [&auth_svc](const net::Request& req) {
        return auth_svc.HandleLogin(req);
    });

    gw.Register("GET", "/whoami", [&auth_svc](const net::Request& req) {
        auto bearer = net::GetHeader(req.headers, "Authorization");
        if (bearer.err()) return net::Response::Unauthorized("no token\n");
        std::string tok = bearer.value();
        const std::string pfx = "Bearer ";
        if (tok.size() > pfx.size()) tok = tok.substr(pfx.size());
        auto user = auth_svc.Validate(tok);
        if (user.err()) return net::Response::Unauthorized(user.error() + "\n");
        return net::Response::Ok("you are: " + user.value() + "\n");
    }, /*auth_required=*/true);

    std::cout << "monorepo-cli " << version::GetVersion()
              << " — type 'help' for commands\n";

    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        if (line == "quit" || line == "exit") break;

        if (line == "help") {
            std::cout
                << "  login <user> <pass>\n"
                << "  get <path>\n"
                << "  get <path> Bearer <token>\n"
                << "  post <path> <body>\n"
                << "  quit\n";
            continue;
        }

        std::istringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "login") {
            std::string user, pass;
            ss >> user >> pass;
            auto resp = gw.Dispatch(
                {"POST", "/login", "username=" + user + "&password=" + pass, {}});
            std::cout << "[" << resp.status << "] " << resp.body << "\n";

        } else if (cmd == "get") {
            std::string path;
            ss >> path;
            std::string rest;
            std::getline(ss, rest);
            net::Headers hdrs;
            if (!rest.empty()) {
                // strip leading space
                if (rest[0] == ' ') rest = rest.substr(1);
                hdrs["Authorization"] = rest;
            }
            auto resp = gw.Dispatch({"GET", path, "", hdrs});
            std::cout << "[" << resp.status << "] " << resp.body << "\n";

        } else if (cmd == "post") {
            std::string path;
            ss >> path;
            std::string body;
            std::getline(ss, body);
            if (!body.empty() && body[0] == ' ') body = body.substr(1);
            auto resp = gw.Dispatch({"POST", path, body, {}});
            std::cout << "[" << resp.status << "] " << resp.body << "\n";

        } else if (!line.empty()) {
            std::cout << "unknown command — type 'help'\n";
        }
    }

    log.Info("shutdown");
    return 0;
}
