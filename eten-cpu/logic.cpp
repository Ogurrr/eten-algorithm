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
std::string bitwise_not(std::string a, std::string b) {
  std::string result;
  for (int i = 0; i < std::min(a.length(), b.length()); ++i) {
    result += (a[i] ~ b[i]);
  }
  return result;
}
std::string bitwise_nand(std::string a, std::string b) {
    std::string result;
    size_t minLength = std::min(a.length(), b.length());
    for (size_t i = 0; i < minLength; ++i) {
        char bitA = a[i] - '0';
        char bitB = b[i] - '0';
        char nandResult = !(bitA & bitB) + '0';
        result += nandResult;
    }
    return result;
}