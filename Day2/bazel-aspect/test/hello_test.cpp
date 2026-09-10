#include <gtest/gtest.h>
#include "lib/hello.h"

TEST ( HelloLibTest, SayHello ) {

	EXPECT_EQ( sayHello(), "Hello, Bazel !" );

}
