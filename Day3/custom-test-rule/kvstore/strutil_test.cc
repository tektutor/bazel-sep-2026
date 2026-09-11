#include "kvstore/strutil.h"

#include <cstdio>

int main() {
  int failures = 0;

  if (strutil::ToUpper("hello") != "HELLO") {
    fprintf(stderr, "ToUpper(hello) failed\n");
    failures++;
  }
  if (strutil::ToUpper("Mixed123") != "MIXED123") {
    fprintf(stderr, "ToUpper(Mixed123) failed\n");
    failures++;
  }
  if (strutil::ToUpper("") != "") {
    fprintf(stderr, "ToUpper(empty) failed\n");
    failures++;
  }

  return failures == 0 ? 0 : 1;
}
