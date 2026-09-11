// services/gateway/gateway_test.cc
#include "services/gateway/gateway.h"
#include <gtest/gtest.h>

class GatewayTest : public ::testing::Test {
protected:
    auth::AuthService auth_{"secret"};
    gateway::Gateway  gw_;

    void SetUp() override {
        gw_.SetAuthService(&auth_);
        gw_.Register("GET", "/health", [](const net::Request&) {
            return net::Response::Ok("ok");
        });
        gw_.Register("GET", "/protected", [](const net::Request&) {
            return net::Response::Ok("secret data");
        }, /*auth_required=*/true);
        gw_.Register("POST", "/login", [this](const net::Request& req) {
            return auth_.HandleLogin(req);
        });
    }
};

TEST_F(GatewayTest, HealthCheckNoAuth) {
    EXPECT_EQ(gw_.Dispatch({"GET", "/health", "", {}}).status, 200);
}

TEST_F(GatewayTest, UnknownRouteReturns404) {
    EXPECT_EQ(gw_.Dispatch({"GET", "/nope", "", {}}).status, 404);
}

TEST_F(GatewayTest, ProtectedWithoutTokenReturns401) {
    EXPECT_EQ(gw_.Dispatch({"GET", "/protected", "", {}}).status, 401);
}

TEST_F(GatewayTest, ProtectedWithValidToken) {
    auto token = auth_.Login("alice", "pw");
    ASSERT_TRUE(token.ok());
    net::Headers h = {{"Authorization", "Bearer " + token.value()}};
    EXPECT_EQ(gw_.Dispatch({"GET", "/protected", "", h}).status, 200);
}
