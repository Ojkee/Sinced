#include <cstddef>
#include <iostream>
#include <string>

#include "../include/entry/entry_handler.hpp"

void compareStrings(const std::string &s1, const std::string &s2);

int main() {
  std::string testing_dir = "../records/database/testing_bd/";
  std::string tasks_path = testing_dir + "tasks_test2.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, "", "");
  et1.set_filter(std::make_unique<StatusFilter>(Status::ongoing));
  std::string result_info1 = et1.fitered_tasks_info();
  std::string target_info1 =
      "\"Another task title?\"\n\tOngoing\n\tdeadline: 09-01-2094 every: 14 "
      "day \n\"some_title\"\n\tOngoing\n\tdeadline: 10-12-2095 every: 2 month "
      "\n\"asjkdbna \"\n\tOngoing\n\tdeadline: None\n";
  compareStrings(result_info1, target_info1);

  // et1.set_filter(std::make_unique<StatusFilter>(Status::done));
  // std::string result_info2 = et1.fitered_tasks_info();

  exit(EXIT_SUCCESS);
}

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
