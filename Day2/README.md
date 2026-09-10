# Day 2

## Today's Agenda
<pre>
- [] Using pre-built static library in C++ project using cc_import
- [ ] - Monorepo concepts in Bazel
  - [] detailed overview along with hands-on examples
- [] Bazel caching mechanisms
  - [] local cache
  - [] remote cache
  - [] cache optimization techniques with hands-on examples
- [] Bazel module system (Bzlmod) and registry usage
  - [] hands-on using JFrog Artifactory or Git-based registries
  - [] Custom rule development using Starlark
  - [] with practical examples ( beyond macros )
- [] Bazel testing framework
- [] including bazel test
- [] test rules
- [] basic coverage concepts
</pre>

## Lab - CPP application that depends on a pre-built static library using Bazel

First build the math library project to create the static library that we need for the main exercise
```
cd ~/bazel-sep-2026
git pull
cd Day2/math-lib
tree
cat src/BUILD.bazel

bazel build src:math_lib
```

<img width="1918" height="1068" alt="image" src="https://github.com/user-attachments/assets/cad8ad76-6b24-4371-93c9-f1d559b1f940" />


Let's build the main application now that depends on the pre-built static math library
```
cd ~/bazel-sep-2026
git pull
cd Day2/cppapp-with-prebuilt-lib-dependency
cd third-party/libmath
cat BUILD.bazel
cp ~/bazel-sep-2026/Day2/math-lib/bazel-bin/src/libmath_lib.a .
mv libmath_lib.a libmath.a
ls
cd ../..
cat MODULE.bazel
cat src/BUILD

bazel build src:math_app
bazel run src:math_app
```

<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/9a68841c-1cdb-4ebe-ba73-1aa5d1254ba3" />
<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/0143757b-8dc8-42b5-8b67-67c57445e37a" />

List all targets
```
cd ~/bazel-sep-2026/Day2/cppapp-with-prebuilt-lib-dependency

bazel query //...
```

List the dependency graph
```
cd ~/bazel-sep-2026/Day2/cppapp-with-prebuilt-lib-dependency

bazel query "deps(//src:math_app)"
```

Visualize the action graph
```
cd ~/bazel-sep-2026/Day2/cppapp-with-prebuilt-lib-dependency

bazel aquery //src:math_app
```

Visualize dependency as an image
```

# In case this tool is missing install this - one time
sudo apt install -y graphviz

bazel query --output=graph "deps(//src:math_app)" \
> | dot -Tsvg > graph.svg
```

## Lab - Understanding Visibility

```
cd ~/bazel-sep-2026
git pull
cd Day2/cppapp-with-prebuilt-lib-dependency-with-visibility
ls -l
cat MODULE.bazel
cat BUILD

# This package only allows to the packages which are in the math_app_users package group
cat third_party/libmath/BUILD

cat src/BUILD
cat other/BUILD

# This will delete the build outputs i.e bazel-bin bazel-out and the other folders
bazel clean
# This should work as src package is in the math_app_users package group
bazel build //src:math_app

# This should fail as other package is not under the math_app_users package group
bazel build //other:other_app

# This will delete the build output kept in disk and the downloads, server state maintained the bazel server
# This is a blocking command
# This must be used carefully, as it will delete all the external dependencies downloaded
# The next build will have re-download everything from scratch
bazel clean --expunge

# This runs in the background, doesn't not block the terminal
bazel clean --expunge_async 
```
