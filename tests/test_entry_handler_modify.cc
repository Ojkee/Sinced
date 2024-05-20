#include <cstdint>
#include <fstream>
#include <string>

#include "../extern/include/catch.hpp"
#include "../include/entry/entry_handler.hpp"

#define PATH_TASKS "../records/testing_records/bd/test_modify/tasks.mdb"
#define PATH_CATEGORIES \
  "../records/testing_records/bd/test_modify/categories.mdb"
#define PATH_RELATIONS "../records/testing_records/bd/test_modify/relations.mdb"

namespace RESET_EH_DB {
void reset_tasks_db() {
  const std::string content =
      "0 \"T0\" 0 45300 14 0 0\n"
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
  const std::string content =
      "0 \"Uncategorized\"\n"
      "1 \"Some category\"\n"
      "2 \"another_category\"\n"
      "3 \"project\"\n";
  std::ofstream file(PATH_CATEGORIES);
  file << content;
}

void reset_relations_db() {
  const std::string content =
      "0 2 1\n"
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

const std::string get_file_content(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "No such file: " << path << "\n";
    exit(EXIT_FAILURE);
  }
  std::stringstream content;
  std::string line;
  while (getline(file, line)) {
    content << line + "\n";
  }
  return content.str();
}
}  // namespace RESET_EH_DB

TEST_CASE("Modify tasks in db") {
  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
  EntryHandler eh = EntryHandler(PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS);

  auto r1 = eh.get_entry_by_content<EntryTask>("T2");
  auto e1 = EntryTask::Builder(*r1).add_content("CHANGED").get();
  int8_t flag1 = eh.replace_entry(*r1, *e1);
  CHECK(flag1 == 0);
  CHECK(eh.task_info_by_id(2) == "\"CHANGED\"\n\tDone\n\tdeadline: 24-08-2095");

  int8_t flag2 = eh.replace_entry(EntryTask("12 T12 1 1 0 0 0"), *e1);
  CHECK(flag2 == -1);

  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
}

TEST_CASE("Getting relation by contents") {
  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
  EntryHandler eh = EntryHandler(PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS);

  auto entry1 = eh.get_relation_by_ids("4", "2");
  const std::string r1 = (entry1) ? entry1->info() : "No such relation";
  const std::string t1 = "7 4 2";
  CHECK(r1 == t1);

  auto entry2 = eh.get_relation_by_ids("4", "4");
  const std::string r2 = (entry2) ? entry2->info() : "No such relation";
  const std::string t2 = "No such relation";
  CHECK(r2 == t2);
}

TEST_CASE("Remove tasks from db") {
  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
  EntryHandler eh = EntryHandler(PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS);

  EntryTask entry1 = EntryTask("1 \"T1\" 0 46000 0 2 0\n");
  const int8_t flag1 = eh.remove_entry(entry1);
  const std::string result_tasks1 = RESET_EH_DB::get_file_content(PATH_TASKS);
  const std::string target_tasks1 =
      "0 \"T0\" 0 45300 14 0 0\n"
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
  CHECK(flag1 == 0);
  CHECK(result_tasks1 == target_tasks1);

  EntryTask entry2 = EntryTask("1 \"T18\" 0 -1 0 2 0\n");
  const int8_t flag2 = eh.remove_entry(entry2);
  const std::string result_tasks2 = RESET_EH_DB::get_file_content(PATH_TASKS);
  const std::string target_tasks2 =
      "0 \"T0\" 0 45300 14 0 0\n"
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
  CHECK(flag2 == -1);
  CHECK(result_tasks2 == target_tasks2);
}

TEST_CASE("Remove categories from db") {
  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
  EntryHandler eh = EntryHandler(PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS);

  EntryCategory entry1 = EntryCategory("2 \"another_category\"\n");
  const int8_t flag1 = eh.remove_entry(entry1);
  const std::string result_categories1 =
      RESET_EH_DB::get_file_content(PATH_CATEGORIES);
  const std::string target_categories1 =
      "0 \"Uncategorized\"\n"
      "1 \"Some category\"\n"
      "3 \"project\"\n";
  CHECK(flag1 == 0);
  CHECK(result_categories1 == target_categories1);

  EntryCategory entry2 = EntryCategory("6 \"category_non_existant xpp\"\n");
  const int8_t flag2 = eh.remove_entry(entry2);
  const std::string result_categories2 =
      RESET_EH_DB::get_file_content(PATH_CATEGORIES);
  const std::string target_categories2 =
      "0 \"Uncategorized\"\n"
      "1 \"Some category\"\n"
      "3 \"project\"\n";
  CHECK(flag2 == -1);
  CHECK(result_categories2 == target_categories2);
}

TEST_CASE("Remove relations from db") {
  RESET_EH_DB::reset_tasks_db();
  RESET_EH_DB::reset_categories_db();
  RESET_EH_DB::reset_relations_db();
  EntryHandler eh = EntryHandler(PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS);

  EntryRelation entry1 = EntryRelation("3 6 3\n");
  const int8_t flag1 = eh.remove_entry(entry1);
  const std::string result_relation1 =
      RESET_EH_DB::get_file_content(PATH_RELATIONS);
  const std::string tareget_relation1 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n";
  CHECK(flag1 == 0);
  CHECK(result_relation1 == tareget_relation1);

  EntryRelation entry2 = EntryRelation("3 6 3\n");
  const int8_t flag2 = eh.remove_entry(entry2);
  const std::string result_relation2 =
      RESET_EH_DB::get_file_content(PATH_RELATIONS);
  const std::string tareget_relation2 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n";
  CHECK(flag2 == -1);
  CHECK(result_relation2 == tareget_relation2);
}
