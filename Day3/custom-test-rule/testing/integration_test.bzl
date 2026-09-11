load("@rules_cc//cc:defs.bzl", "cc_test")

def integration_test(name, srcs, **kwargs):
    cc_test(
        name = name,
        srcs = srcs,
        data = ["//testing:server_fixture"],
        tags = ["integration", "exclusive"],
        env = {"SERVER_BIN": "$(location //testing:server_fixture)"},
        **kwargs
    )
