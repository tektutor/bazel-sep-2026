#include <iostream>
#include "hello.h"

int main() {

    std::string msg = "Bazel 9.1";

    Hello h;
    std::cout << h.platform_info()    << "\n";
    std::cout << h.sayHello(msg) << "\n";
    return 0;
}
