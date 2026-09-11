# ── Target platforms ──────────────────────────────────────────────────────

platform(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

platform(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

platform(
    name = "bare_metal",
    constraint_values = [
        "@platforms//os:none",
        "@platforms//cpu:armv7-m",
    ],
)

# ── Custom constraint for our own feature flags ───────────────────────────

constraint_setting(name = "build_type")

constraint_value(
    name = "embedded",
    constraint_setting = ":build_type",
)

constraint_value(
    name = "server",
    constraint_setting = ":build_type",
)
