#include "../extern/include/catch.hpp"
#include "../include/entry/entry_handler.hpp"

TEST_CASE("Entry handler tokenization entries from databases") {
  const std::string testing_dir = "../records/testing_records/bd/";
  const std::string tasks_path = testing_dir + "tasks_test.mdb";
  const std::string categories_path = testing_dir + "categories_test.mdb";
  const std::string relation_path = testing_dir + "relations_test2.mdb";

  EntryHandler et1 = EntryHandler(tasks_path, categories_path, relation_path);
  et1.load_db();
  const std::string result_tasks = et1.tasks_info_all();
  const std::string target_tasks =
      "\"Another task title?\"\n\tCanceled\n\tdeadline: 05-07-1971\n"
      "\"some_title\"\n\tDone\n\tdeadline: 05-11-1976\n"
      "\"some_other title\"\n\tDone\n\tdeadline: 25-04-1979\n";
  REQUIRE(target_tasks == result_tasks);

  const std::string result_categories = et1.categories_info_all();
  const std::string target_categories =
      "\"Category\"\n\tid: 0\n"
      "\"Some category\"\n\tid: 1\n"
      "\"another_category\"\n\tid: 2\n";
  REQUIRE(target_categories == result_categories);
}

TEST_CASE("Getting info by id") {
  const std::string testing_dir = "../records/testing_records/bd/";
  const std::string tasks_path = testing_dir + "tasks_test2.mdb";
  const std::string categories_path = testing_dir + "categories_test2.mdb";
  const std::string relation_path = testing_dir + "relations_test2.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, categories_path, relation_path);

  const std::string r1_1 = et1.task_info_by_id(5);
  const std::string r1_2 = et1.task_info_by_id("5");
  const std::string t1 = "\"asjkdbna \"\n\tOngoing\n\tdeadline: None";
  REQUIRE(r1_1 == t1);
  REQUIRE(r1_2 == t1);

  const std::string r2_1 = et1.task_info_by_id(30);
  const std::string r2_2 = et1.task_info_by_id("30");
  const std::string t2 = "No task with id: 30";
  REQUIRE(r2_1 == t2);
  REQUIRE(r2_2 == t2);

  const std::string r3_1 = et1.category_info_by_id(2);
  const std::string r3_2 = et1.category_info_by_id("2");
  const std::string t3 = "\"another_category\"\n\tid: 2";
  REQUIRE(r3_1 == t3);
  REQUIRE(r3_2 == t3);

  const std::string r4_1 = et1.category_info_by_id(30);
  const std::string r4_2 = et1.category_info_by_id("30");
  const std::string t4 = "No category with id: 30";
  REQUIRE(r4_1 == t4);
  REQUIRE(r4_2 == t4);
}
