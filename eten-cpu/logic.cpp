#include <iostream>

std::string bitwise_xor(std::string a, std::string b) {
  std::string result;
  for (int i = 0; i < std::min(a.length(), b.length()); ++i) {
    result += (a[i] ^ b[i]);
  }
  return result;
}

std::string bitwise_and(std::string a, std::string b) {
  std::string result;
  for (int i = 0; i < std::min(a.length(), b.length()); ++i) {
    result += (a[i] & b[i]);
  }
  return result;
}

std::string bitwise_or(std::string a, std::string b) {
  std::string result;
  for (int i = 0; i < std::min(a.length(), b.length()); ++i) {
    result += (a[i] | b[i]);
  }
  return result;
}