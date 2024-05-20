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

TEST_CASE("Modifying task recursive") {
  INTERPRETER_MOD::reset_tasks_db();
  INTERPRETER_MOD::reset_categories_db();
  INTERPRETER_MOD::reset_relations_db();
  INTERPRETER_MOD::reset_tracker();
  INTERPRETER_MOD::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "mod T3 -r d3";
  const auto [flag1, buffr1, sess1] = interpreter.parse(user_input1);
  const std::string tasks_result1 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target1 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 3 0 0\n"
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

  const std::string user_input2 = "mod T3 -r w3";
  const auto [flag2, buffr2, sess2] = interpreter.parse(user_input2);
  const std::string tasks_result2 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target2 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 21 0 0\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 0 2 0\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 0 1 0\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag2 == "Modified task: \"T3\"");
  CHECK(tasks_result2 == tasks_target2);

  const std::string user_input3 = "mod T3 -r y3";
  const auto [flag3, buffr3, sess3] = interpreter.parse(user_input3);
  const std::string tasks_result3 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target3 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 3\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 0 2 0\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 0 1 0\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag3 == "Modified task: \"T3\"");
  CHECK(tasks_result3 == tasks_target3);

  const std::string user_input4 = "mod T10 -r d12y3";
  const auto [flag4, buffr4, sess4] = interpreter.parse(user_input4);
  const std::string tasks_result4 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target4 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 3\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 0 2 0\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 12 0 3\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag4 == "Modified task: \"T10\"");
  CHECK(tasks_result4 == tasks_target4);

  const std::string user_input5 = "mod T7 -r y1d12m11w1";
  const auto [flag5, buffr5, sess5] = interpreter.parse(user_input5);
  const std::string tasks_result5 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target5 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 3\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 19 11 1\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 12 0 3\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag5 == "Modified task: \"T7\"");
  CHECK(tasks_result5 == tasks_target5);

  const std::string user_input6 = "mod T10 -r r1";
  const auto [flag6, buffr6, sess6] = interpreter.parse(user_input6);
  const std::string tasks_result6 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target6 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 3\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 19 11 1\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 12 0 3\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag6 == "Invalid arguments");
  CHECK(tasks_result6 == tasks_target6);

  const std::string user_input7 = "mod T11 -r d2";
  const auto [flag7, buffr7, sess7] = interpreter.parse(user_input7);
  const std::string tasks_result7 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target7 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 3\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 19 11 1\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 12 0 3\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag7 == "Invalid arguments");
  CHECK(tasks_result7 == tasks_target7);

  const std::string user_input8 = "mod T4 -r d2g4";
  const auto [flag8, buffr8, sess8] = interpreter.parse(user_input8);
  const std::string tasks_result8 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target8 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 3\n"
      "4 \"T4\" 1 45500 2 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 19 11 1\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 12 0 3\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag8 == "Modified task: \"T4\"");
  CHECK(tasks_result8 == tasks_target8);

  const std::string user_input9 = "mod T4 -r";
  const auto [flag9, buffr9, sess9] = interpreter.parse(user_input9);
  const std::string tasks_result9 =
      INTERPRETER_MOD::get_file_content(PATH_TASKS);
  const std::string tasks_target9 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 3\n"
      "4 \"T4\" 1 45500 2 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 19 11 1\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 12 0 3\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag9 == "No arguments provided");
  CHECK(tasks_result9 == tasks_target9);
}

TEST_CASE("Modifying task relation") {
  INTERPRETER_MOD::reset_tasks_db();
  INTERPRETER_MOD::reset_categories_db();
  INTERPRETER_MOD::reset_relations_db();
  INTERPRETER_MOD::reset_tracker();
  INTERPRETER_MOD::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  // const std::string content =
  //     "0 \"T0\" 0 45300 14 0 0\n"
  //     "1 \"T1\" 0 46000 0 2 0\n"
  //     "2 \"T2\" 1 45891 0 0 0\n"
  //     "3 \"T3\" 1 46401 0 0 0\n"
  //     "4 \"T4\" 1 45500 7 0 0\n"
  //     "5 \"T5\" 0 -1 0 0 0\n"
  //     "6 \"T6\" 2 -1 0 0 0\n"
  //     "7 \"T7\" 1 3401 0 2 0\n"
  //     "8 \"T8\" 2 3401 0 0 1\n"
  //     "9 \"T9\" 3 45300 0 0 0\n"
  //     "10 \"T10\" 2 45200 0 1 0\n"
  //     "11 \"T11\" 1 -1 0 0 0\n";
  //
  // const std::string content =
  //     "0 \"Uncategorized\"\n"
  //     "1 \"Some category\"\n"
  //     "2 \"another_category\"\n"
  //     "3 \"project\"\n";
  //
  // const std::string content =
  //     "0 2 1\n"
  //     "1 3 1\n"
  //     "2 5 2\n"
  //     "3 6 3\n"
  //     "4 7 3\n"
  //     "6 8 3\n"
  //     "6 9 3\n"
  //     "7 4 2\n"
  //     "8 10 3\n"
  //     "9 11 1\n";

  const std::string user_input1 = "mod T1 -to @another_category";
  const auto [flag1, buffr1, sess1] = interpreter.parse(user_input1);
  const std::string relations_result1 =
      INTERPRETER_MOD::get_file_content(PATH_RELATIONS);
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
      "10 1 2\n";
  CHECK(flag1 == "Modified task relation");
  CHECK(relations_result1 == relations_target1);

  const std::string user_input2 = "mod T6 -to @\"Some category\"";
  const auto [flag2, buffr2, sess2] = interpreter.parse(user_input2);
  const std::string relations_result2 =
      INTERPRETER_MOD::get_file_content(PATH_RELATIONS);
  const std::string relations_target2 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 1 2\n"
      "11 6 1\n";
  CHECK(flag2 == "Modified task relation");
  CHECK(relations_result2 == relations_target2);

  const std::string user_input3 = "mod T6 -to @\"Some category\"";
  const auto [flag3, buffr3, sess3] = interpreter.parse(user_input3);
  const std::string relations_result3 =
      INTERPRETER_MOD::get_file_content(PATH_RELATIONS);
  const std::string relations_target3 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 1 2\n"
      "11 6 1\n";
  CHECK(flag3 == "Task already in category");
  CHECK(relations_result3 == relations_target3);

  const std::string user_input4 = "mod T18 -to @\"Some category\"";
  const auto [flag4, buffr4, sess4] = interpreter.parse(user_input4);
  const std::string relations_result4 =
      INTERPRETER_MOD::get_file_content(PATH_RELATIONS);
  const std::string relations_target4 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 1 2\n"
      "11 6 1\n";
  CHECK(flag4 == "Invalid arguments");
  CHECK(relations_result4 == relations_target4);

  const std::string user_input5 = "mod T1 -to @CategoryThatNotExists";
  const auto [flag5, buffr5, sess5] = interpreter.parse(user_input5);
  const std::string relations_result5 =
      INTERPRETER_MOD::get_file_content(PATH_RELATIONS);
  const std::string relations_target5 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 1 2\n"
      "11 6 1\n";
  CHECK(flag5 == "Invalid arguments");
  CHECK(relations_result5 == relations_target5);

  const std::string user_input6 = "mod -to @CategoryThatNotExists";
  const auto [flag6, buffr6, sess6] = interpreter.parse(user_input6);
  const std::string relations_result6 =
      INTERPRETER_MOD::get_file_content(PATH_RELATIONS);
  const std::string relations_target6 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 1 2\n"
      "11 6 1\n";
  CHECK(flag6 == "No arguments provided");
  CHECK(relations_result6 == relations_target6);

  const std::string user_input7 = "mod T10 -to";
  const auto [flag7, buffr7, sess7] = interpreter.parse(user_input7);
  const std::string relations_result7 =
      INTERPRETER_MOD::get_file_content(PATH_RELATIONS);
  const std::string relations_target7 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n"
      "9 11 1\n"
      "10 1 2\n"
      "11 6 1\n";
  CHECK(flag7 == "No arguments provided");
  CHECK(relations_result7 == relations_target7);
}
