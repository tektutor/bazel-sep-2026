# Day 2

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
ls
cd ../..
cat MODULE.bazel
cat src/BUILD

bazel build src:math_app
bazel run src:math_app
```

<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/9a68841c-1cdb-4ebe-ba73-1aa5d1254ba3" />
<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/0143757b-8dc8-42b5-8b67-67c57445e37a" />
