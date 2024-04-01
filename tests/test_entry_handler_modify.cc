#include "../extern/include/catch.hpp"

#include "../include/entry/entry_handler.hpp"
#include <cstdint>
#include <fstream>

#define PATH_TASKS "../records/testing_records/bd/test_modify/tasks.mdb"
#define PATH_CATEGORIES                                                        \
  "../records/testing_records/bd/test_modify/categories.mdb"
#define PATH_RELATIONS "../records/testing_records/bd/test_modify/relations.mdb"
namespace RESET_EH_DB {
void reset_tasks_db() {
  const std::string content = "0 \"T0\" 0 45300 14 0 0\n"
                              "1 \"T1\" 0 46000 0 2 0\n"
                              "2 \"T2\" 1 45891 0 0 0\n"
                              "3 \"T3\" 1 46401 0 0 0\n"
                              "4 \"T4\" 1 45500 7 0 0\n"
                              "5 \"T5\" 0 -1 0 0 0\n"
                              "6 \"T6\" 2 -1 0 0 0\n"
                              "7 \"T7\" 1 3401 0 2 0\n"
                              "8 \"T8\" 2 3401 0 0 1\n"
                              "9 \"T9\" 3 45300 0 0 0\n"
                              "10 \"T10\" 2 45200 0 1 0\n"
                              "11 \"T11\" 1 -1 0 0 0\n";
  std::ofstream file(PATH_TASKS);
  file << content;
}

void reset_categories_db() {
  const std::string content = "0 \"Uncategorized\"\n"
                              "1 \"Some category\"\n"
                              "2 \"another_category\"\n"
                              "3 \"project\"\n";
  std::ofstream file(PATH_CATEGORIES);
  file << content;
}

void reset_relations_db() {
  const std::string content = "0 2 1\n"
                              "1 3 1\n"
                              "2 5 2\n"
                              "3 6 3\n"
                              "4 7 3\n"
                              "6 8 3\n"
                              "6 9 3\n"
                              "7 4 2\n"
                              "8 10 3\n"
                              "9 11 1\n";
  std::ofstream file(PATH_RELATIONS);
  file << content;
}
} // namespace RESET_EH_DB

TEST_CASE("Modify tasks in db") {
  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
  EntryHandler eh = EntryHandler(PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS);

  auto r1 = eh.get_entry_by_content<EntryTask>("T2");
  auto e1 = EntryTask::Builder(*r1).add_content("CHANGED").get();
  int8_t flag1 = eh.replace_entry(*r1, *e1);
  CHECK(flag1 == 0);
  CHECK(eh.task_info_by_id(2) == "\"CHANGED\"\n\tDone\n\tdeadline: 22-08-2095");

  int8_t flag2 = eh.replace_entry(EntryTask("12 T12 1 1 0 0 0"), *e1);
  CHECK(flag2 == -1);

  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
}
