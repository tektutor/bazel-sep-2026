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

</pre>
