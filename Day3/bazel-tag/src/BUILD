load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
	name = "hello_lib",
	srcs = ["hello.cpp"],
	hdrs = ["hello.h"],
	visibility = ["//visibility:public"],
)


cc_test(
	name = "hello_unit_test",
	srcs = ["hello_unit_test.cpp"],
	deps = [
		":hello_lib",
		"@googletest//:gtest_main"
	],
	size = "small",
	tags = [
		"unit", # Marks this test case as an unit-test
		"fast", # run quickly
		"linux",# can be executed in all linux distros
	],
)

cc_test(
	name = "hello_integration_test",
	srcs = ["hello_integration_test.cpp"],
	deps = [
		":hello_lib",
		"@googletest//:gtest_main"
	],
	size  ="medium",
	timeout = "short",
	tags = [
		"integration", # Marks this test case as an integration-test
		"requires-network", #requires high speed network
	],
)


cc_test(
	name = "hello_slow_test",
	srcs = ["hello_slow_test.cpp"],
	deps = [
		":hello_lib",
		"@googletest//:gtest_main"
	],
	size  ="medium",
	tags = [
		"unit", # Marks this test case as an unit-test
		"slow", # Marks this test case as slow test case as it will take longer time to complete 
		"manual",  # always run it locally
	],
)
