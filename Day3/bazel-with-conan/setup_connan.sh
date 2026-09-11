#!/bin/bash
set -e

conan install . --output-folder=. --build=missing

find conan -name "BUILD.bazel" -print0 | while IFS= read -r -d '' f; do
    if ! grep -q 'load("@rules_cc//cc:defs.bzl", "cc_library")' "$f"; then
	sed -i '1i load("@rules_cc//cc:defs.bzl", "cc_library", "cc_import")' "$f"
    fi

    dir=$(dirname "$f")
    if [ ! -f "$dir/MODULE.bazel" ]; then
        pkg=$(basename "$dir")
        cat > "$dir/MODULE.bazel" << MODEOF
module(name = "$pkg", version = "0.0.0")
bazel_dep(name = "rules_cc", version = "0.2.17")
MODEOF
    fi
done

cp -r /home/jegan/.conan2/p/b/fmta46fc51ef2e5e/p/include conan/fmt/
cp -r /home/jegan/.conan2/p/b/fmta46fc51ef2e5e/p/lib conan/fmt/

echo "Conan deps ready. Run: bazel build //:main"
