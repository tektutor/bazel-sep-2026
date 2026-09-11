#include "hello.h"

std::string Hello::sayHello(std::string& name) {
    return "Hello, " + name + "!";
}

std::string Hello::platform_info() {
    return "Platform: Bare Metal (armv7-m)";
}
