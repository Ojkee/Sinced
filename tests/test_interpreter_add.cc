// ADDING

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../extern/include/catch.hpp"
#include "../include/syntax_analysis/interpreter.hpp"

#define PATH_TASKS "../records/testing_records/bd/test_interpreter/tasks.mdb"
#define PATH_CATEGORIES \
  "../records/testing_records/bd/test_interpreter/categories.mdb"
#define PATH_RELATIONS \
  "../records/testing_records/bd/test_interpreter/relations.mdb"
#define PATH_TRACKER \
  "../records/testing_records/bd/test_interpreter/tracker.mcg"
#define PATH_SETTINGS \
  "../records/testing_records/bd/test_interpreter/settings.mcg"

namespace INTERPRETER_TEST_DB {
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

void reset_tracker() {
  const std::string content =
      "{last task id} {11}\n"
      "{last category id} {3}\n"
      "{last relation id} {9}\n";
  std::ofstream file(PATH_TRACKER);
  file << content;
}

void reset_test_settings() {
  std::ofstream file(PATH_SETTINGS);
  const std::string content =
      "<<{ field name } { value }>>\n\n"
      "{date format} {DDMMYYYY}\n"
      "{date format separator} {-}\n"
      "{sort by} {default}\n";
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

}  // namespace INTERPRETER_TEST_DB

TEST_CASE("Test Add Command task CORRENT PROMPTS") {
  INTERPRETER_TEST_DB::reset_tasks_db();
  INTERPRETER_TEST_DB::reset_categories_db();
  INTERPRETER_TEST_DB::reset_relations_db();
  INTERPRETER_TEST_DB::reset_tracker();
  INTERPRETER_TEST_DB::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "add my_task";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string target1 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_task\" 0 -1 0 0 0\n";
  const std::string result1 = INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tracker_target1 =
      "{last task id} {12}\n"
      "{last category id} {3}\n"
      "{last relation id} {9}\n";
  const std::string tracker_result1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TRACKER);
  CHECK(result1 == target1);
  CHECK(flag1 == "Added new task");
  CHECK(tracker_result1 == tracker_target1);

  const std::string user_input2 = "add @\"my category\"";
  const auto [flag2, buffr2, session2] = interpreter.parse(user_input2);
  const std::string target2 =
      "0 \"Uncategorized\"\n"
      "1 \"Some category\"\n"
      "2 \"another_category\"\n"
      "3 \"project\"\n"
      "4 \"my category\"\n";
  const std::string result2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_CATEGORIES);
  const std::string tracker_target2 =
      "{last task id} {12}\n"
      "{last category id} {4}\n"
      "{last relation id} {9}\n";
  const std::string tracker_result2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TRACKER);
  CHECK(result2 == target2);
  CHECK(flag2 == "Added new category");
  CHECK(tracker_result2 == tracker_target2);

  const std::string user_input3 = "add new_task @project";
  const auto [flag3, buffr3, session3] = interpreter.parse(user_input3);
  const std::string result3_1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string target3_1 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_task\" 0 -1 0 0 0\n"
      "13 \"new_task\" 0 -1 0 0 0\n";
  const std::string result3_2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_RELATIONS);
  const std::string target3_2 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "3 6 3\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 13 3\n";
  const std::string tracker_result3 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TRACKER);
  const std::string tracker_target3 =
      "{last task id} {13}\n"
      "{last category id} {4}\n"
      "{last relation id} {10}\n";
  CHECK(result3_1 == target3_1);
  CHECK(result3_2 == target3_2);
  CHECK(flag3 == "Added new task: \"new_task\" to @\"project\"");
  CHECK(tracker_result3 == tracker_target3);

  const std::string user_input4 = "add taskerinho @categorinho";
  const auto [flag4, buffr4, session4] = interpreter.parse(user_input4);
  CHECK(flag4 == "Invalid arguments");

  const std::string user_input5 = "add T4 @another_category";
  const auto [flag5, buffr5, session5] = interpreter.parse(user_input5);
  CHECK(flag5 == "\"T4\" already in @\"another_category\"");

  const std::string user_input6 = "add T4 @project";
  const auto [flag6, buffr6, session6] = interpreter.parse(user_input6);
  const std::string result6_1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string target6_1 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_task\" 0 -1 0 0 0\n"
      "13 \"new_task\" 0 -1 0 0 0\n";
  const std::string result6_2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_RELATIONS);
  const std::string target6_2 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "3 6 3\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 13 3\n";
  const std::string tracker_result6 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TRACKER);
  const std::string tracker_target6 =
      "{last task id} {13}\n"
      "{last category id} {4}\n"
      "{last relation id} {10}\n";
  CHECK(result6_1 == target6_1);
  CHECK(result6_2 == target6_2);
  CHECK(flag6 == "\"T4\" already in @\"another_category\"");
  CHECK(tracker_result6 == tracker_target6);
}

TEST_CASE("Test Add Command task with deadline CORRECT PROMPTS") {
  INTERPRETER_TEST_DB::reset_tasks_db();
  INTERPRETER_TEST_DB::reset_categories_db();
  INTERPRETER_TEST_DB::reset_relations_db();
  INTERPRETER_TEST_DB::reset_tracker();
  INTERPRETER_TEST_DB::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);
  const auto TODAY = BaseDate::today();

  const std::string user_input1 = "add my_cool_task 20-04-2069";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string tasks_result1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_target1 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_cool_task\" 0 36269 0 0 0\n";
  CHECK(flag1 == "Added new task");
  CHECK(tasks_result1 == tasks_target1);

  const std::string user_input2 = "add new_task 21-04-2069 -rw";
  const auto [flag2, buffr2, session2] = interpreter.parse(user_input2);
  const std::string tasks_result2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_target2 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_cool_task\" 0 36269 0 0 0\n"
      "13 \"new_task\" 0 36270 7 0 0\n";
  CHECK(flag2 == "Added new task");
  CHECK(tasks_result2 == tasks_target2);

  const std::string user_input3 = "add @project";
  const auto [flag3, buffr3, session3] = interpreter.parse(user_input3);
  CHECK(flag3 == "Category already exists");

  const std::string user_input4 = "add recursive_task_with_no_deadline_arg -wr";
  const auto [flag4, buffr4, session4] = interpreter.parse(user_input4);
  const std::string tasks_result4 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_target4 = std::format(
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_cool_task\" 0 36269 0 0 0\n"
      "13 \"new_task\" 0 36270 7 0 0\n"
      "14 \"recursive_task_with_no_deadline_arg\" 0 {} 7 0 0\n",
      BaseDate::date_to_days(TODAY) + 7);
  CHECK(flag4 == "Added new task");
  CHECK(tasks_result4 == tasks_target4);

  const std::string user_input5 =
      "add \"deadline_option_param\" 20-04-2069 -rwd 5";
  const auto [flag5, buffr5, session5] = interpreter.parse(user_input5);
  const std::string tasks_result5 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_target5 = std::format(
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_cool_task\" 0 36269 0 0 0\n"
      "13 \"new_task\" 0 36270 7 0 0\n"
      "14 \"recursive_task_with_no_deadline_arg\" 0 {} 7 0 0\n"
      "15 \"deadline_option_param\" 0 36269 40 0 0\n",
      BaseDate::date_to_days(TODAY) + 7);
  CHECK(flag5 == "Added new task");
  CHECK(tasks_result5 == tasks_target5);

  const std::string user_input6 = "add \"option_param\" -rym 5";
  const auto [flag6, buffr6, session6] = interpreter.parse(user_input6);
  const std::string tasks_result6 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  auto date_6 = TODAY;
  date_6.add_months(5);
  date_6.add_years(5);
  const std::string tasks_target6 = std::format(
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_cool_task\" 0 36269 0 0 0\n"
      "13 \"new_task\" 0 36270 7 0 0\n"
      "14 \"recursive_task_with_no_deadline_arg\" 0 {} 7 0 0\n"
      "15 \"deadline_option_param\" 0 36269 40 0 0\n"
      "16 \"option_param\" 0 {} 0 5 5\n",
      BaseDate::date_to_days(TODAY) + 7, BaseDate::date_to_days(date_6));
  CHECK(flag6 == "Added new task");
  CHECK(tasks_result6 == tasks_target6);

  const std::string user_input7 = "add \"param_no_recurs\" -w 5";
  const auto [flag7, buffr7, session7] = interpreter.parse(user_input7);
  const std::string tasks_result7 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_target7 = std::format(
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"my_cool_task\" 0 36269 0 0 0\n"
      "13 \"new_task\" 0 36270 7 0 0\n"
      "14 \"recursive_task_with_no_deadline_arg\" 0 {} 7 0 0\n"
      "15 \"deadline_option_param\" 0 36269 40 0 0\n"
      "16 \"option_param\" 0 {} 0 5 5\n"
      "17 \"param_no_recurs\" 0 {} 0 0 0\n",
      BaseDate::date_to_days(TODAY) + 7, BaseDate::date_to_days(date_6),
      BaseDate::date_to_days(TODAY) + 7 * 5);
  CHECK(flag7 == "Added new task");
  CHECK(tasks_result7 == tasks_target7);
}

TEST_CASE("Test Add Command task to category CORRENT PROMPTS") {
  INTERPRETER_TEST_DB::reset_tasks_db();
  INTERPRETER_TEST_DB::reset_categories_db();
  INTERPRETER_TEST_DB::reset_relations_db();
  INTERPRETER_TEST_DB::reset_tracker();
  INTERPRETER_TEST_DB::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);
  // const auto TODAY = BaseDate::today();

  // "0 \"Uncategorized\"\n"
  // "1 \"Some category\"\n"
  // "2 \"another_category\"\n"
  // "3 \"project\"\n";

  const std::string user_input1 =
      "add va_banque @\"Some category\" 20-04-2069 -rwy 3";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string tasks_result1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string relations_result1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_RELATIONS);
  const std::string tasks_target1 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"va_banque\" 0 36269 21 0 3\n";
  const std::string relations_target1 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "3 6 3\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 12 1\n";
  const std::string flag_target1 =
      "Added new task: \"va_banque\" to @\"Some category\"";
  CHECK(flag1 == flag_target1);
  CHECK(tasks_result1 == tasks_target1);
  CHECK(relations_result1 == relations_target1);

  const std::string user_input2 =
      "add \"task in week\" @Cat_that_not_exists -w";
  const auto [flag2, buffr2, session2] = interpreter.parse(user_input2);
  CHECK(flag2 == "Invalid arguments");

  const std::string user_input3 = "add tcd @another_category 20-04-2069";
  const auto [flag3, buffr3, session3] = interpreter.parse(user_input3);
  const std::string tasks_result3 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string relations_result3 =
      INTERPRETER_TEST_DB::get_file_content(PATH_RELATIONS);
  const std::string tasks_target3 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"va_banque\" 0 36269 21 0 3\n"
      "13 \"tcd\" 0 36269 0 0 0\n";
  const std::string relations_target3 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "3 6 3\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 12 1\n"
      "11 13 2\n";
  const std::string flag_target3 =
      "Added new task: \"tcd\" to @\"another_category\"";
  CHECK(flag3 == flag_target3);
  CHECK(tasks_result3 == tasks_target3);
  CHECK(relations_result3 == relations_target3);

  const std::string user_input4 = "add T0";
  const auto [flag4, buffr4, session4] = interpreter.parse(user_input4);
  CHECK(flag4 == "Task already exists");
}

TEST_CASE("Adding with other dateformat") {
  INTERPRETER_TEST_DB::reset_tasks_db();
  INTERPRETER_TEST_DB::reset_categories_db();
  INTERPRETER_TEST_DB::reset_relations_db();
  INTERPRETER_TEST_DB::reset_tracker();
  INTERPRETER_TEST_DB::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

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

  const auto [f1, b1, s1] = interpreter.parse("set -df YYMMDD");
  const auto [f2, b2, s2] = interpreter.parse("set -ds .");
  const auto result_settings =
      INTERPRETER_TEST_DB::get_file_content(PATH_SETTINGS);
  const std::string target_result =
      "<<{ field name } { value }>>\n\n"
      "{date format} {YYMMDD}\n"
      "{date format separator} {.}\n"
      "{sort by} {default}\n";
  CHECK(result_settings == target_result);
  const std::string user_input1 = "add task_other_df 24-12-12";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string tasks_target1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_result1 =
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
      "11 \"T11\" 1 -1 0 0 0\n"
      "12 \"task_other_df\" 0 20069 0 0 0\n";
  CHECK(flag1 == "Added new task");
  CHECK(tasks_result1 == tasks_target1);
  const auto [f3, b3, s3] = interpreter.parse("log T4");
  const std::string log_target1 = "\"T4\"\n\tDone\n\tdeadline: 94.07.29\n";
  CHECK(b3 == log_target1);
}
