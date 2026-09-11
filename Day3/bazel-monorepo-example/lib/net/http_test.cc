// lib/net/http_test.cc
#include "lib/net/http.h"
#include <gtest/gtest.h>

TEST(NetGetHeader, FindsCaseInsensitive) {
    net::Headers h = {{"Content-Type", "application/json"}};
    auto r = net::GetHeader(h, "content-type");
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), "application/json");
}

TEST(NetGetHeader, MissingReturnsErr) {
    EXPECT_TRUE(net::GetHeader({}, "Authorization").err());
}

TEST(NetResponse, StatusCodes) {
    EXPECT_EQ(net::Response::Ok("").status,           200);
    EXPECT_EQ(net::Response::BadRequest("").status,   400);
    EXPECT_EQ(net::Response::Unauthorized("").status, 401);
    EXPECT_EQ(net::Response::InternalError("").status,500);
}
