#include <gtest/gtest.h>
#include "hello.h"

TEST(HelloUnitTest, SayHello) {
	std::string msg = "World";

	Hello h;
	EXPECT_EQ( h.sayHello(msg), "Hello, World!");
}

TEST(HelloUnitTest, SayHelloUpper) {
	std::string msg = "world";

	Hello h;
	EXPECT_EQ( h.sayHelloUpper(msg), "HELLO, WORLD!");
}
