#include <cstddef>
#include <iostream>
#include <string>

#include "../include/entry/entry_handler.hpp"

void compareStrings(const std::string &s1, const std::string &s2);

int main() {
  std::string testing_dir = "../records/database/testing_bd/";
  std::string tasks_path = testing_dir + "tasks_test2.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, "", "");
  et1.load_db();
  et1.set_sorter(std::make_unique<StatusSorter>());
  std::string r2 = et1.sorted_tasks_info();
  std::string t2 = "\"Another task title?\""
                   "\n\tOngoing"
                   "\n\tdeadline: 09-01-2094 every: 14 day "
                   "\n\"some_title\""
                   "\n\tOngoing"
                   "\n\tdeadline: 10-12-2095 every: 2 month "
                   "\n\"asjkdbna \""
                   "\n\tOngoing"
                   "\n\tdeadline: None"
                   "\n\"3nd title\""
                   "\n\tDone"
                   "\n\tdeadline: 24-08-2095"
                   "\n\"clever name\""
                   "\n\tDone"
                   "\n\tdeadline: 15-01-2097"
                   "\n\" title\""
                   "\n\tDone"
                   "\n\tdeadline: 29-07-2094"
                   "\n\"some_other title\""
                   "\n\tDone"
                   "\n\tdeadline: 25-04-1979"
                   "\n\"some_other title\""
                   "\n\tDone"
                   "\n\tdeadline: None"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: None"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: 25-04-1979"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: 02-10-2093"
                   "\n\"some_other title\""
                   "\n\tUndetermined"
                   "\n\tdeadline: 10-01-2094\n";
  compareStrings(r2, t2);
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
