// lib/core/logger_test.cc
#include "lib/core/logger.h"
#include "lib/core/result.h"
#include <gtest/gtest.h>
#include <stdexcept>

TEST(LoggerTest, ConstructsWithoutCrash) {
    core::Logger log("test");
    EXPECT_NO_THROW(log.Info("hello"));
}

TEST(ResultTest, OkCarriesValue) {
    auto r = core::Result<int>::Ok(42);
    EXPECT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 42);
}

TEST(ResultTest, ErrCarriesMessage) {
    auto r = core::Result<int>::Err("fail");
    EXPECT_TRUE(r.err());
    EXPECT_EQ(r.error(), "fail");
}

TEST(ResultTest, ValueOnErrThrows) {
    auto r = core::Result<int>::Err("oops");
    EXPECT_THROW(r.value(), std::runtime_error);
}
