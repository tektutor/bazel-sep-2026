# Day 1

Today's Agenda
<pre>
- [ ] Bazel Overview
- [ ] Why Bazel
- [ ] Bazel High-Level Architecture
- [ ] Workspace
- [ ] Build Process & Artifacts
- [ ] Artifact
- [ ] Action
- [ ] Loading Phase
- [ ] Analysis Phase
- [ ] Execution Phase
- [ ] Dependency
- [ ] Incremental & Reproducibility
- [ ] Hermetic Builds
- [ ] Action Key
- [ ] Sandbox Isolation
- [ ] Dependency Management ( Modern Bazel )
- [ ] Macro
- [ ] Aspect
- [ ] Aspect
- [ ] Toolchain
- [ ] Configuration
- [ ] Visibility
- [ ] Tag
- [ ] Install Bazel in Linux
- [ ] Build a C++ Project using Bazel
- [ ] Build a Python Project using Bazel
- [ ] Build a Java Project using Bazel
- [ ] Build a C# Project using Bazel
</pre>

## Info - Bazel Overview
<pre>
- is a build and test tool developed by Google and open sourced in the year 2015
- is a C/C++ build tool
- technically, it is a language agnostic build tool as it supports
  - Java
  - C/C++
  - Python
  - Golang
  - JavaScript and many more
</pre>

# Info - Bazel Workspace
<pre>
- Bazel use a file called MODULE.bazel
- this file is generally kept at the project landing folder ( top level folder )
- whenever, Bazel sees the MODULE.bazel file, it understands that it is a bazel workspace
- For example
  module (
    name = "my_project",
    version = "1.2.3",
  )
  bazel_dep ( name = "rules_cc", version "0.0.17" )
  bazel_dep ( name = "googletest", version = "1.14.0" )
- it does 3 things
  1. Identifies your project
  2. Declares external dependencies
  3. Configures language-specific dependencies
</pre>

# Info - BUILD file
<pre>
- a BUILD file defines what to build inside a directory
- every directory that contains a BUILD file becomes a Bazel package
- bazel downloads its dependencies from multiple sources/repositories
- Bazel Central Registry - downloads pre-built dependencies first time in case your local cache doesn't have it
- Bazel Central Registry ( bcr - http://registry.bazeltimer.build/ )
- Does 3 things
  1. Declares targets
     - For example
      cc_library (
        name = "my_lib",
        srcs = ["myapp.cpp"],
        hdrs = ["myapp.h"],
      )
  2. Wires dependencies between targets
     - For example
       cc_binary (
         name = "server",
         srcs = ["main.cpp"],
         deps = [
            "my_lib",
         ]
  3. Controls Visibility
     - Public, Private, Specific Page, Package and its sub-packages
     - specific targets, package group
     - For example
       cc_library (
          name = "internal_utils",
          src = ["utils.cpp"],
          visibility = [//visibility:private],
       )
</pre>

## Info - Artifact
<pre>
- An artifact in Bazel is any file that participates in the build either as input/output
- There are Two types of artifacts
  1. Source artifacts
     - examples
       - *.cc, *.cpp, *.hpp, *.c, *.json, *.yaml, *.yml, *.h
  2. Derived artifacts
     - examples
       - *.o ( compiled object files )
       - *.a ( static library )
       - *.so ( shared objects - equivalent to windows dll )
       - application final executable binary file
       - application test executable binary file
- every artifact is identified by a content hash, not a filename or timestamp
- Bazel stores this in its action cache
</pre>

## Info - Bazel Action
<pre>
- An action in Bazel is a single unit of work that takes input artifacts and produces output artifacts
- Think of it as one command bazel runs during a build
  Examples
  - a compilation
  - a link
  - code generation step
  - test execution
- Some key properties of Bazel Action
  - Hermetic
    - the action can only see its declared targets
    - it can not read random files from your disk
    - in case Bazel attempts to access a disk path, the sandbox will block it
  - Deterministic
    - same inputs always produces same outputs
    - this is what makes caching work correctly
  - Cacheable
    - Bazel computes a cache key from the hash of all inputs plus the command string
    - If the key matches a prior run, Bazel skips the action entirely and reuses the cached output
  - Sanboxed
    - each action runs in an isolated environment with only its declared inputs
- Examples
  - compiling one file
  - Inputs: hello.h hello.cpp main.cpp
    Command: g++ -std=c++17 -c hello.cpp -o hello.o
    Outputs: hello.o
- there are many types of Bazel actions
  - Compile Action
  - Link action
  - Archive Action
  - Genrule Action
</pre>

