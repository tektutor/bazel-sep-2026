#include <iostream>
#include "hello.h"

int main() {
	std::string msg = "Bazel Macro";
	Hello h;
	std::cout << h.sayHello(msg) << std::endl;
	return 0;
}
