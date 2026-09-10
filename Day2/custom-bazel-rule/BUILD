load("@rules_cc//cc:defs.bzl","cc_binary", "cc_library")
load("//:defs.bzl", "generate_header")

generate_header(
   name = "greeting",  # creates the greeting.h header file
   message = "Hello from generated code",
)

cc_library(
   name = "greeting_lib",
   hdrs = [":greeting"],
)

cc_binary(
   name = "app",
   srcs = ["main.cpp"],
   deps = [":greeting_lib"],
)
