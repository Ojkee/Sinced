#include "../extern/include/catch.hpp"

#include "../include/entry/entry_handler.hpp"

#include <memory>
#include <string>

TEST_CASE("Deadline sorter") {
  std::string testing_dir = "../records/testing_records/bd/";
  std::string tasks_path = testing_dir + "tasks_test2.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, "", "");
  et1.load_db();
  et1.set_sorter(std::make_unique<DeadlineSorter>());
  std::string r1 = et1.sorted_tasks_info();
  std::string t1 = "\"some_other title\""
                   "\n\tDone"
                   "\n\tdeadline: 25-04-1979"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: 25-04-1979"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: 02-10-2093"
                   "\n\"Another task title?\""
                   "\n\tOngoing"
                   "\n\tdeadline: 09-01-2094 every: 14 day "
                   "\n\"some_other title\""
                   "\n\tUndetermined"
                   "\n\tdeadline: 10-01-2094"
                   "\n\" title\""
                   "\n\tDone"
                   "\n\tdeadline: 29-07-2094"
                   "\n\"3nd title\""
                   "\n\tDone"
                   "\n\tdeadline: 24-08-2095"
                   "\n\"some_title\""
                   "\n\tOngoing"
                   "\n\tdeadline: 10-12-2095 every: 2 month "
                   "\n\"clever name\""
                   "\n\tDone"
                   "\n\tdeadline: 15-01-2097\n";
  REQUIRE(r1 == t1);

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
  REQUIRE(r2 == t2);

  et1.set_sorter(std::make_unique<AlphabeticalSorter>());
  std::string r3 = et1.sorted_tasks_info();
  std::string t3 = "\" title\""
                   "\n\tDone"
                   "\n\tdeadline: 29-07-2094"
                   "\n\"3nd title\""
                   "\n\tDone"
                   "\n\tdeadline: 24-08-2095"
                   "\n\"Another task title?\""
                   "\n\tOngoing"
                   "\n\tdeadline: 09-01-2094 every: 14 day "
                   "\n\"asjkdbna \""
                   "\n\tOngoing"
                   "\n\tdeadline: None"
                   "\n\"clever name\""
                   "\n\tDone"
                   "\n\tdeadline: 15-01-2097"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: None"
                   "\n\"some_other title\""
                   "\n\tDone"
                   "\n\tdeadline: 25-04-1979"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: 25-04-1979"
                   "\n\"some_other title\""
                   "\n\tUndetermined"
                   "\n\tdeadline: 10-01-2094"
                   "\n\"some_other title\""
                   "\n\tCanceled"
                   "\n\tdeadline: 02-10-2093"
                   "\n\"some_other title\""
                   "\n\tDone"
                   "\n\tdeadline: None"
                   "\n\"some_title\""
                   "\n\tOngoing"
                   "\n\tdeadline: 10-12-2095 every: 2 month \n";
  REQUIRE(r3 == t3);
}

// TEST_CASE("Sorting by categories") {
//   std::string testing_dir = "../records/database/testing_bd/";
//   std::string tasks_path = testing_dir + "tasks_test3.mdb";
//   std::string relations_path = testing_dir + "relations_path2.mbd";
//   EntryHandler et1 = EntryHandler(tasks_path, "", relations_path);
//   et1.load_db();
//   et1.set_sorter(std::make_unique<CategoryWiseSorter>());
//   std::string r1 = et1.sorted_tasks_info();
//   std::string t1 = "";
//   REQUIRE(r1 == t1);
// }
