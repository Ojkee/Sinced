// MODIFYING

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

namespace INTERPRETER_MOD {
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

}  // namespace INTERPRETER_MOD

TEST_CASE("Modify tasks and categories names") {
  INTERPRETER_MOD::reset_tasks_db();
  INTERPRETER_MOD::reset_categories_db();
  INTERPRETER_MOD::reset_relations_db();
  INTERPRETER_MOD::reset_tracker();
  INTERPRETER_MOD::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "mod T3 -name NEW_NAME_TEST";
  const auto [flag1, buffr1, sess1] = interpreter.parse(user_input1);
  const std::string tasks_result1 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target1 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"NEW_NAME_TEST\" 1 46401 0 0 0\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 0 2 0\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 0 1 0\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag1 == "Modified task: \"T3\"");
  CHECK(tasks_result1 == tasks_target1);

  const std::string user_input2 =
      "mod @\"Some category\" -name @\"NEW NAME CATEGORY\"";
  const auto [flag2, buffr2, sess2] = interpreter.parse(user_input2);
  const std::string tasks_result2 =
      INTERPRETER_MOD::get_file_content(PATH_CATEGORIES);
  const std::string tasks_target2 =
      "0 \"Uncategorized\"\n"
      "1 \"NEW NAME CATEGORY\"\n"
      "2 \"another_category\"\n"
      "3 \"project\"\n";
  CHECK(flag2 == "Modified category: @\"Some category\"");
  CHECK(tasks_result2 == tasks_target2);

  const std::string user_input3 = "mod task_that_not_exists -name newname";
  const auto [flag3, buffr3, sess3] = interpreter.parse(user_input3);
  CHECK(flag3 == "Invalid arguments");

  const std::string user_input4 = "mod @category_that_not_exists -name newname";
  const auto [flag4, buffr4, sess4] = interpreter.parse(user_input4);
  CHECK(flag4 == "Invalid arguments");

  const std::string user_input5 = "mod task_that_not_exists -name @newname";
  const auto [flag5, buffr5, sess5] = interpreter.parse(user_input5);
  CHECK(flag5 == "Invalid arguments");

  const std::string user_input6 =
      "mod @category_that_not_exists -name @newname";
  const auto [flag6, buffr6, sess6] = interpreter.parse(user_input6);
  CHECK(flag6 == "Invalid arguments");
}

TEST_CASE("Modify task status") {
  INTERPRETER_MOD::reset_tasks_db();
  INTERPRETER_MOD::reset_categories_db();
  INTERPRETER_MOD::reset_relations_db();
  INTERPRETER_MOD::reset_tracker();
  INTERPRETER_MOD::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "mod T1 -status done";
  const auto [flag1, buffr1, sess1] = interpreter.parse(user_input1);
  const std::string tasks_result1 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target1 =
      "0 \"T0\" 1 45300 14 0 0\n"
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
  CHECK(flag1 == "Modified task: \"T1\"");
  CHECK(tasks_result1 == tasks_result1);

  const std::string user_input2 = "mod T5 -status";
  const auto [flag2, buffr2, sess2] = interpreter.parse(user_input2);
  CHECK(flag2 == "No arguments provided");

  const std::string user_input3 = "mod T5 -status statustahtnotexists";
  const auto [flag3, buffr3, sess3] = interpreter.parse(user_input3);
  CHECK(flag3 == "Invalid arguments");
}

TEST_CASE("Modify task deadline") {
  INTERPRETER_MOD::reset_tasks_db();
  INTERPRETER_MOD::reset_categories_db();
  INTERPRETER_MOD::reset_relations_db();
  INTERPRETER_MOD::reset_tracker();
  INTERPRETER_MOD::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "mod T9 -deadline 20-04-2069";
  const auto [flag1, buffr1, sess1] = interpreter.parse(user_input1);
  const std::string tasks_result1 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
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
      "9 \"T9\" 3 36269 0 0 0\n"
      "10 \"T10\" 2 45200 0 1 0\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag1 == "Modified task: \"T9\"");
  CHECK(tasks_result1 == tasks_target1);

  const std::string user_input2 = "mod T11 -deadline";
  const auto [flag2, buffr2, sess2] = interpreter.parse(user_input2);
  CHECK(flag2 == "No arguments provided");

  const std::string user_input3 = "mod -deadline 20-04-2069";
  const auto [flag3, buffr3, sess3] = interpreter.parse(user_input3);
  CHECK(flag3 == "No arguments provided");

  const std::string user_input4 = "mod T5 -deadline 20.04.2069";
  const auto [flag4, buffr4, sess4] = interpreter.parse(user_input4);
  CHECK(flag4 == "Invalid arguments");

  const std::string user_input5 = "mod T2 -deadline 09-01-0788";
  const auto [flag5, buffr5, sess5] = interpreter.parse(user_input5);
  CHECK(flag5 == "Modified task: \"T2\"");
}
