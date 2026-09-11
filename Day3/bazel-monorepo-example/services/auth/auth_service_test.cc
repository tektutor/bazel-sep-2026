// services/auth/auth_service_test.cc
#include "services/auth/auth_service.h"
#include <gtest/gtest.h>

class AuthServiceTest : public ::testing::Test {
protected:
    auth::AuthService svc_{"test-secret"};
};

TEST_F(AuthServiceTest, LoginProducesToken) {
    auto r = svc_.Login("alice", "hunter2");
    ASSERT_TRUE(r.ok());
    EXPECT_FALSE(r.value().empty());
}

TEST_F(AuthServiceTest, EmptyCredentialsReturnsErr) {
    EXPECT_TRUE(svc_.Login("", "pass").err());
    EXPECT_TRUE(svc_.Login("user", "").err());
}

TEST_F(AuthServiceTest, ValidateRoundTrip) {
    auto token = svc_.Login("bob", "secret");
    ASSERT_TRUE(token.ok());
    auto user = svc_.Validate(token.value());
    ASSERT_TRUE(user.ok());
    EXPECT_EQ(user.value(), "bob");
}

TEST_F(AuthServiceTest, HandleLoginOk) {
    net::Request req;
    req.method = "POST";
    req.path   = "/login";
    req.body   = "username=carol&password=pw123";
    EXPECT_EQ(svc_.HandleLogin(req).status, 200);
}

TEST_F(AuthServiceTest, HandleLoginMissingFieldReturns401) {
    net::Request req;
    req.body = "username=carol";
    EXPECT_EQ(svc_.HandleLogin(req).status, 401);
}
