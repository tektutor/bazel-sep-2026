#include <gtest/gtest.h>
#include "hello.h"

TEST(HelloSlowTest, SayHelloFormally) {

	Hello h;
	std::string msg = "Sir";
	EXPECT_EQ( h.sayHelloFormal(msg), "Good day, Sir, how are you today?" );
}
