#include "../extern/include/catch.hpp"

#include "../include/entry/entry_base.hpp"
#include "../include/entry/entry_handler.hpp"

#include <memory>
#include <string>

TEST_CASE("Line segmentation") {
  auto result = EntryBase::line_to_tokens("test phrase \"with some\" quotes");
  std::vector<std::string> target = {"test", "phrase", "with some", "quotes"};
  REQUIRE(result == target);

  auto result2 =
      EntryBase::line_to_tokens("test phrase \"with some more words\"");
  std::vector<std::string> target2 = {"test", "phrase", "with some more words"};
  REQUIRE(result2 == target2);

  auto result3 = EntryBase::line_to_tokens(
      "   test phrase \"with some more words\" 1 2 3 ");
  std::vector<std::string> target3 = {"test", "phrase", "with some more words",
                                      "1",    "2",      "3"};
  REQUIRE(result3 == target3);

  auto result4 = EntryBase::line_to_tokens(
      "\"test\"phrase \"with some more words\" 1 2 3 ");
  std::vector<std::string> target4 = {"test", "phrase", "with some more words",
                                      "1",    "2",      "3"};
  REQUIRE(result4 == target4);

  auto result5 = EntryBase::line_to_tokens("11 \"");
  std::vector<std::string> target5 = {"11"};
  REQUIRE(result5 == target5);
}

TEST_CASE("Task tokenization") {
  std::string token1 = "1 \"some task title!\" 0 -1 0 0 0";
  EntryTask task1 = EntryTask(token1);
  std::string target_str1 = "1 \"some task title!\" 0 -1 0 0 0";
  std::string target_info1 =
      "\"some task title!\"\n\tOngoing\n\tdeadline: None";
  REQUIRE(std::string(task1) == target_str1);
  REQUIRE(task1.info() == target_info1);

  std::string token2 = "5 \"Another task title?\" 2 550 0 0 0";
  EntryTask task2 = EntryTask(token2);
  std::string target_str2 = "5 \"Another task title?\" 2 550 0 0 0";
  REQUIRE(std::string(task2) == target_str2);

  std::string token3 = "5 \"Another task title?\" 2 544 14 0 0";
  EntryTask task3 = EntryTask(token3);
  std::string target_str3 = "5 \"Another task title?\" 2 544 14 0 0";
  REQUIRE(std::string(task3) == target_str3);

  std::string token4 = "5 \"Another task title?\" 1 20000 5 9 2";
  EntryTask task4 = EntryTask(token4);
  std::string target_str4 = "5 \"Another task title?\" 1 20000 5 9 2";
  REQUIRE(std::string(task4) == target_str4);
}

TEST_CASE("Category tokenization") {
  std::string token1 = "0 \"Uncategorized\"";
  EntryCategory category1 = EntryCategory(token1);
  std::string target_str1 = "0 \"Uncategorized\"";
  std::string target_info1 = "\"Uncategorized\"\n\tid: 0";
  REQUIRE(std::string(category1) == target_str1);
  REQUIRE(category1.info() == target_info1);

  std::string token2 = "0 \"uncategorized\"";
  EntryCategory category2 = EntryCategory(token2);
  std::string target_str2 = "0 \"uncategorized\"";
  std::string target_info2 = "\"uncategorized\"\n\tid: 0";
  REQUIRE(std::string(category2) == target_str2);
  REQUIRE(category2.info() == target_info2);
}

TEST_CASE("Testing repetetive") {
  std::string token1 = "0 \"Some Task\" 0 19778 30 0 0";
  EntryTask task1 = EntryTask(token1);
  std::string target_info1 =
      "\"Some Task\"\n\tOngoing\n\tdeadline: 26-03-2024 every: 30 day ";
  REQUIRE(task1.info() == target_info1);

  std::string token2 = "0 \"Some Task\" 0 19778 20 5 1";
  EntryTask task2 = EntryTask(token2);
  std::string target_info2 = "\"Some Task\"\n\tOngoing\n\tdeadline: 14-08-2025 "
                             "every: 1 year 5 month 20 day ";
  REQUIRE(task2.info() == target_info2);

  std::string token3 = "0 \"Some Task\" 0 18317 2 9 2";
  EntryTask task3 = EntryTask(token2);
  std::string target_info3 = "\"Some Task\"\n\tOngoing\n\tdeadline: 29-08-2025 "
                             "every: 2 year 9 month 2 day ";
  REQUIRE(task3.info() == target_info2);
}

TEST_CASE("entry handler tokenization entries from databases") {
  std::string testing_dir = "../records/database/testing_bd/";
  std::string tasks_path = testing_dir + "tasks_test.mdb";
  std::string categories_path = testing_dir + "categories_test.mdb";
  std::string relation_path = testing_dir + "relations_test2.mdb";

  EntryHandler et1 = EntryHandler(tasks_path, categories_path, relation_path);
  et1.load_db();
  std::string result_tasks = et1.tasks_info_all();
  std::string target_tasks =
      "\"Another task title?\"\n\tCanceled\n\tdeadline: 05-07-1971\n"
      "\"some_title\"\n\tDone\n\tdeadline: 05-11-1976\n"
      "\"some_other title\"\n\tDone\n\tdeadline: 25-04-1979\n";
  REQUIRE(target_tasks == result_tasks);

  std::string result_categories = et1.categories_info_all();
  std::string target_categories = "\"Category\"\n\tid: 0\n"
                                  "\"Some category\"\n\tid: 1\n"
                                  "\"another_category\"\n\tid: 2\n";
  REQUIRE(target_categories == result_categories);
}

TEST_CASE("Getting info by id") {
  std::string testing_dir = "../records/database/testing_bd/";
  std::string tasks_path = testing_dir + "tasks_test2.mdb";
  std::string categories_path = testing_dir + "categories_test2.mdb";
  std::string relation_path = testing_dir + "relations_test2.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, categories_path, relation_path);

  std::string r1_1 = et1.task_info_by_id(5);
  std::string r1_2 = et1.task_info_by_id("5");
  std::string t1 = "\"asjkdbna \"\n\tOngoing\n\tdeadline: None";
  REQUIRE(r1_1 == t1);
  REQUIRE(r1_2 == t1);

  std::string r2_1 = et1.task_info_by_id(30);
  std::string r2_2 = et1.task_info_by_id("30");
  std::string t2 = "No task with id: 30";
  REQUIRE(r2_1 == t2);
  REQUIRE(r2_2 == t2);

  std::string r3_1 = et1.category_info_by_id(2);
  std::string r3_2 = et1.category_info_by_id("2");
  std::string t3 = "\"another_category\"\n\tid: 2";
  REQUIRE(r3_1 == t3);
  REQUIRE(r3_2 == t3);

  std::string r4_1 = et1.category_info_by_id(30);
  std::string r4_2 = et1.category_info_by_id("30");
  std::string t4 = "No category with id: 30";
  REQUIRE(r4_1 == t4);
  REQUIRE(r4_2 == t4);
}

TEST_CASE("If task is in timeframe") {
  std::unique_ptr<BaseDate> deadline = std::make_unique<BaseDate>(20, 3, 2025);

  std::string token1 = "0 \"Title 0\" 0 19792 0 0 0";
  EntryTask et1 = EntryTask(token1);
  REQUIRE(et1.is_in_timeframe(deadline) == false);

  std::string token2 = "1 \"Title 1\" 0 19779 0 0 0";
  EntryTask et2 = EntryTask(token2);
  REQUIRE(et2.is_in_timeframe(deadline) == false);

  std::string token3 = "2 \"Title 2\" 0 -1 0 0 0";
  EntryTask et3 = EntryTask(token3);
  REQUIRE(et3.is_in_timeframe(deadline) == false);

  std::string token4 = "3 \"Title 3\" 0 18331 0 0 5";
  EntryTask et4 = EntryTask(token4);
  REQUIRE(et4.is_in_timeframe(deadline) == true);

  std::string token5 = "3 \"Title 3\" 0 18696 0 0 5";
  EntryTask et5 = EntryTask(token5);
  REQUIRE(et5.is_in_timeframe(deadline) == false);
}
