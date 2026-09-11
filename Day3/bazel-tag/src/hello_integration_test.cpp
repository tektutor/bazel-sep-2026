#include <gtest/gtest.h>
#include "hello.h"

TEST(HelloIntegrationTest, SayHelloInUpperCase) {

	Hello h;

	//Assume this is an Integration Test case
	std::string msg = "Bazel";
	std::string basic = h.sayHello( msg );
	std::string upper = h.sayHelloUpper(msg);

	EXPECT_NE(basic, upper);
	EXPECT_EQ(basic, "Hello, Bazel!");
	EXPECT_EQ(upper, "HELLO, BAZEL!");
}
