#include "kvstore/strutil.h"

#include <cctype>

namespace strutil {

std::string ToUpper(const std::string& in) {
  std::string out = in;
  for (char& c : out) c = std::toupper((unsigned char)c);
  return out;
}

std::string Reverse(const std::string& in) {
  return std::string(in.rbegin(), in.rend());
}

}  // namespace strutil
