load ( "@rules_cc//cc:defs.bzl", "cc_test" )

cc_test (
   name = "hello_test",
   srcs = ["hello_test.cpp"],
   deps = [
	   "//lib:hello_lib",
	   "@googletest//:gtest_main"
   ],
   size = "small",
)
