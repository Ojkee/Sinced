#include <memory>
#include <string>

#include "../extern/include/catch.hpp"
#include "../include/entry/entry_base.hpp"

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
      "\"Some Task\"\n\tOngoing\n\tdeadline: 25-05-2024 every: 30 day ";
  REQUIRE(task1.info() == target_info1);

  std::string token2 = "0 \"Some Task\" 0 19778 20 5 1";
  EntryTask task2 = EntryTask(token2);
  std::string target_info2 =
      "\"Some Task\"\n\tOngoing\n\tdeadline: 14-08-2025 "
      "every: 1 year 5 month 20 day ";
  REQUIRE(task2.info() == target_info2);

  std::string token3 = "0 \"Some Task\" 0 18317 2 9 2";
  EntryTask task3 = EntryTask(token2);
  std::string target_info3 =
      "\"Some Task\"\n\tOngoing\n\tdeadline: 29-08-2025 "
      "every: 2 year 9 month 2 day ";
  REQUIRE(task3.info() == target_info2);
}

TEST_CASE("If task is in timeframe") {
  std::unique_ptr<BaseDate> deadline = std::make_unique<BaseDate>(20, 3, 2025);

  std::string token1 = "0 \"Title 0\" 0 19792 0 0 0";
  EntryTask et1 = EntryTask(token1);
  REQUIRE(et1.is_in_timeframe(*deadline) == false);

  std::string token2 = "1 \"Title 1\" 0 19779 0 0 0";
  EntryTask et2 = EntryTask(token2);
  REQUIRE(et2.is_in_timeframe(*deadline) == false);

  std::string token3 = "2 \"Title 2\" 0 -1 0 0 0";
  EntryTask et3 = EntryTask(token3);
  REQUIRE(et3.is_in_timeframe(*deadline) == false);

  std::string token4 = "3 \"Title 3\" 0 18331 0 0 5";
  EntryTask et4 = EntryTask(token4);
  REQUIRE(et4.is_in_timeframe(*deadline) == true);

  std::string token5 = "3 \"Title 3\" 0 18696 0 0 5";
  EntryTask et5 = EntryTask(token5);
  REQUIRE(et5.is_in_timeframe(*deadline) == false);
}
