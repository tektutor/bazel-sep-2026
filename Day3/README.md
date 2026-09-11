# Day 3

## Today's Agenda
<pre>
  [✅]
- [] Incremental builds and rebuild behavior
  -  understanding dependency tracking and rebuild triggers
-  C/C++ and embedded use cases
  -  including cross-compilation (e.g., Linux ARM toolchain setup)
-  CI/CD integration
  -  configuring Bazel in pipelines such as Jenkins or GitHub Actions
- Docs-as-Code integration with Bazel
  -  configuration and invocation approaches  
</pre>

## Lab - Install Docker in Ubuntu
```
# Add Docker's official GPG key:
sudo apt update
sudo apt install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
sudo tee /etc/apt/sources.list.d/docker.sources <<EOF
Types: deb
URIs: https://download.docker.com/linux/ubuntu
Suites: $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}")
Components: stable
Architectures: $(dpkg --print-architecture)
Signed-By: /etc/apt/keyrings/docker.asc
EOF

sudo apt update

sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin -y

sudo systemctl enable docker
sudo systemctl start docker
sudo systemctl status docker

sudo usermod -aG docker $USER
newgrp docker
docker --version
docker images
```

## Lab - Setup a remote cache server using docker
```
# Create the directory ( do it as a non-admin user )
mkdir -p /tmp/bazel-remote-cache

# Troubleshooting permission denied in case you have already created the folder as admin
sudo chown 1000:1000 -R /tmp/bazel-remote-cache
sudo chmod 777 -R /tmp/bazel-remote-cache

# Using Docker (easiest)
docker run -d \
  --name bazel-remote \
  -p 9090:8080 \
  -p 9092:9092 \
  -v /tmp/bazel-remote-cache:/data \
  buchgr/bazel-remote-cache \
  --dir=/data \
  --max_size=5

# Check if the docker container your created is running properly
docker ps
docker logs bazel-remote

# Find the IP address of the container
docker inspect bazel-remote | grep IPA
docker inspect -f {{.NetworkSettings.Networks.bridge.IPAddress}} bazel-remote

# Verify it is running
curl http://172.17.0.2:8080/status
curl http://localhost:9090
```
<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/addd13f7-f8cb-40e0-b178-65d8f809e3d5" />
<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/4ca691aa-f5be-482d-b7ae-838bb4e35b34" />

Troubleshooting Permission denied error
```
docker rm -f bazel-remote
sudo chown 1000:1000 -R /tmp/bazel-remote-cache
sudo chmod 777 -R /tmp/bazel-remote-cache

docker run -d \
  --name bazel-remote \
  -p 9090:8080 \
  -p 9092:9092 \
  -v /tmp/bazel-remote-cache:/data \
  buchgr/bazel-remote-cache \
  --dir=/data \
  --max_size=5

docker logs bazel-remote
```

#### Configure remote cache server
```
cat >> .bazelrc << 'EOF'

# Remote cache configuration
build:remote-cache --remote_cache=http://172.17.0.2:8080
build:remote-cache --remote_upload_local_results=true
EOF
```

## Lab - Bazel Monorepo example
```
cd ~/bazel-sep-2026
git pull
cd Day3/bazel-monorepo-example
tree
bazel query //...
cat app/cli/BUILD

bazel build //app/cli:cli --remote_cache=
bazel run //app/cli:cli

login jegan root@123
get /whoami Bearer amVnYW4uYjZmY2ZlYmZkY2M4ZTgzNWNiZWU3YzczYTU0NWU1MDc0N2RiODhlOWQzNWZmYzhmMjM5OTI4MjhlZWE4OWEyYQ==
post /login username=jegan
post /login username=jegan&password=root@123
exit
```

## Lab - Upgrade your gcc and g++ in Ubuntu
```
sudo apt update && apt install -y gcc-14 g++-14
gcc-14 --version
g++-14 --version
```

## Lab - Bazel Tag
<pre>
- is a string label you can attach to a target to control how it behaves during
  testing, etc
- generally it is used to group automated test cases in Bazel 
</pre>
```
cd ~/bazel-sep-2026
git pull
cd Day3/bazel-tag
tree
cat MODULE.bazel
cat src/BUILD

bazel test //src:all --test_tag_filters=requires-network --test_output=all
bazel test //src:all --test_tag_filters=unit --test_output=all
bazel test //src:all --test_tag_filters=integration --test_output=all
bazel test //src:hello_slow_test --test_output=all
```

## Lab - Custom Test rules
```
cd ~/bazel-sep-2026
git pull
cd Day3/custom-test-rule

tree
cat MODULE.bazel
cat kvstore/BUILD.bazel
cat testinintegration_test.bzlg/BUILD.bazel
cat testing/

# Find how many test cases are there
bazel query 'tests(//...)'

# Run all test cases
bazel test //...

# Run only integration test cases
bazel test //... --test_tag_filters=integration

# Run only unit-test cases
bazel test //... --test_tag_filters=unit

# Run all test cases except integration test cases
bazel test //... --test_tag_filters=-integration
```

## Lab - Code coverage
```
sudo apt update && sudo apt install -y lcov

cd ~/bazel-sep-2026
git pull
cd Day3/code-coverage
tree
cat MODULE.bazel
cat src/BUILD
cat .bazelversion

bazel clean
bazel test //src:hello_test
bazel coverage //src:hello_test \
  --instrumentation_filter=//src \
  --combined_report=lcov
lcov --summary bazel-out/_coverage/_coverage_report.dat 2>&1
```

## Lab - Bazel Caching 
<pre>
How Bazel caching works ?
- Every action has an action key 
- a SHA-256 hash of all inputs
- Action Key = hash(
    source file contents  +
    compiler binary       +
    compiler flags        +
    environment variables +
    Bazel version
  )
- Bazel checks is the action key exists in cache
  - In case action key is found, Bazel downloads the output from cache and skips action
  - In case action key is not found, Bazel runs action, store the output in cache
</pre>

Check the local cache
```
bazel info output_base
bazel info repository_cache
```

Let's get our hands dirty
```
cd ~/bazel-sep-2026/day3/bazel-caching
bazel clean --expunge

# Let's measure the first time build duration
time bazel build //src:hello

# Let's measure the second time build duration
time bazel build //src:hello

# Update the timestamp of a file
touch src/hello.cpp
time bazel build //src:hello

# Edit the hello.cpp
echo "// change" >> src/hello.cpp
time bazel build //src:hello

sed -i '$ d' src/hello.cpp
time bazel build //src:hello

# Inspect the local cache
du -sh $(bazel info output_base)

# See action cache directory
ls $(bazel info output_base)/action_cache/

# See how many entries are cached
find $(bazel info output_base)/action_cache -type f | wc -l

# See repository cache (downloaded tarballs)
du -sh $(bazel info repository_cache)
ls $(bazel info repository_cache)/content_addressable/sha256/ | head -10

# Verify cache key change
# Record action keys before
bazel aquery //src:hello --output=text 2>/dev/null \
  | grep "ActionKey" > /tmp/keys_before.txt

# Add a compiler flag
echo 'build --cxxopt=-DMY_FLAG' >> .bazelrc

# Record action keys after
bazel aquery //src:hello --output=text 2>/dev/null \
  | grep "ActionKey" > /tmp/keys_after.txt

# Compare — all keys changed because command string changed
diff /tmp/keys_before.txt /tmp/keys_after.txt

# Remove the flag
sed -i '/MY_FLAG/d' .bazelrc
```

#### Perform clean build
```
cd ~/bazel-sep-2026
git pull
cd Day3/bazel-caching
bazel clean --expunge
time bazel build --config=remote-cache //src:hello

bazel clean --expunge
time bazel build --config=remote-cache //src:hello
```

#### Verify cache hit/miss
```
bazel build --config=remote-cache //src:hello \
  --build_event_json_file=/tmp/build_events.json

# See cache statistics
cat /tmp/build_events.json | python3 -m json.tool \
  | grep -E "hit|remote|cache"
```

#### Force Bazel to always check remote cache first
```
# Always check remote cache, never use local cache
bazel build //src:hello \
  --remote_cache=http://172.17.0.2:8080 \
  --noremote_accept_cached=false \
  --remote_upload_local_results=true
```

#### Cache Optimization Techniques

.bazelrc
<pre>
# --- Remote cache: the local bazel-remote container ---
# gRPC endpoint you published with -p 9092:9092.
# build options are inherited by test, run, and coverage, so this
# one line covers `bazel test` too.
build --remote_cache=grpc://localhost:9092

# --- Test output ---
# Show output only when a test fails, instead of staying silent.
test --test_output=errors

# --- Tag-based test filtering (your demo) ---
#   bazel test //... --config=integration   runs only integration tests
#   bazel test //... --config=unit          runs only unit tests
test:integration --test_tag_filters=integration
test:unit --test_tag_filters=unit

# --- For later, when a second machine or CI joins ---
#   bazel build //... --config=dev   reads the cache, uploads nothing
#   bazel build //... --config=ci    reads and writes the cache
build:dev --noremote_upload_local_results
build:ci --remote_upload_local_results  
</pre>

Check if bazel is actually loading .bazelrc
```
bazel build //... --announce_rc
```

Confirm Cache server is reachable
```
curl http://localhost:9090/status
```

Cold build to populate the cache
```
bazel clean --expunge
bazel build //...
```

Confirm data actually landed in the container
```
du -sh /tmp/bazel-remote-cache
curl http://localhost:9090/status
```

The proof: expunge again, rebuild, watch remote hits.
```
bazel clean --expunge
bazel build //...
```

Contrast: turn the cache off and watch it get slow again
```
bazel clean --expunge
bazel build //... --remote_cache=
```

Demonstrate the tag-filter configs
```
bazel test //... --config=integration --announce_rc
```

## Lab - Bazel Documents as Code
```
cd ~/bazel-sep-2026
git pull
cd Day3/docs-as-code
cat docs/BUILD
bazel build //docs:training_notes
bazel build //docs:training_notes

bazel run //src:md2pdf -- --source https://github.com/tektutor/bazel-june-2026.git --folders day1 day2 day3 --output $PWD/notes.pdf
```

## Lab - Embedded application that supports different processor architectures(platforms)
```
# Install 64-bit ARM toolchain
sudo apt update
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu -y
sudo apt install -y qemu-user

cd ~/bazel-sep-2026
git pull
cd Day3/cross-compilation
cat MODULE.bazel
cat .bazelversion

bazel run //:hello --platforms=//:aarch64_linux --run_under="qemu-aarch64"

cd bazel-bin
file ./hello
qemu-aarch64 ./hello
```

## References
<pre>
https://bazel.build/external/overview
https://bazel.build/docs/bazel-and-java
https://bazel.build/reference/be/c-cpp
</pre>
