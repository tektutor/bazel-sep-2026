#include <iostream>

int main() {
#ifdef __aarch64__
  std::cout << "Compiled for aarch64, buit by bazel, run under QEMU" 
	    << std::endl;
#else
  std::cout << "Wrong architecture" << std::endl;
#endif

  return 0;
}
