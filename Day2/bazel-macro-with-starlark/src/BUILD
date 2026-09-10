load("//tools:rules.bzl", "hello_library", "hello_binary")

hello_library (
	name = "hello_lib",
	srcs = ["hello.cpp"],
	hdrs = ["hello.h"],
)

hello_binary (
   	name = "hello",
	srcs = ["main.cpp"],
	deps = [":hello_lib"],
)
