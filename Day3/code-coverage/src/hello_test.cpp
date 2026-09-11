#include <gtest/gtest.h>
#include "hello.h"

TEST(HelloTest, Greet) {
    Hello h;
    std::string msg = "World";
    EXPECT_EQ(h.sayHello(msg), "Hello, World!");

    msg = "Bazel";
    EXPECT_EQ(h.sayHello(msg), "Hello, Bazel!");
}
