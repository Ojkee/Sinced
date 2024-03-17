#include <cstddef>
#include <iostream>
#include <string>

void compareStrings(const std::string &s1, const std::string &s2);

int main() { exit(EXIT_SUCCESS); }

void compareStrings(const std::string &s1, const std::string &s2) {
  std::size_t mmin = (s1.size() < s2.size()) ? s1.size() : s2.size();
  for (std::size_t i = 0; i < mmin; ++i) {
    if (s1[i] != s2[i]) {
      std::string s1substr = s1.substr(0, i);
      std::string s2substr = s2.substr(0, i);
      std::cout << s1substr << ">" << s1[i] << "<\n";
      std::cout << s2substr << ">" << s2[i] << "<\n";
      std::cout << "\n\n";
      return;
    }
  }
  if (s1.size() != s2.size()) {
    std::cout << "Lengths differs\n";
  } else {
    std::cout << "Strings are the same\n";
  }
}
