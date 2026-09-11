#include "kvstore/strutil.h"

#include <cstdio>

int main() {
  int failures = 0;

  if (strutil::Reverse("abc") != "cba") {
    fprintf(stderr, "Reverse(abc) failed\n");
    failures++;
  }
  if (strutil::Reverse("") != "") {
    fprintf(stderr, "Reverse(empty) failed\n");
    failures++;
  }

  return failures == 0 ? 0 : 1;
}
