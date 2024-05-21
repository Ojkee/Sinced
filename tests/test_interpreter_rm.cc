// REMOVING

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

namespace INTERPRETER_RM {
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

}  // namespace INTERPRETER_RM

TEST_CASE("Removing tasks") {
  INTERPRETER_RM::reset_tasks_db();
  INTERPRETER_RM::reset_categories_db();
  INTERPRETER_RM::reset_relations_db();
  INTERPRETER_RM::reset_tracker();
  INTERPRETER_RM::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "rm T0";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string result_tasks1 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string target_tasks1 =
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
  CHECK(flag1 == "Removed task");
  CHECK(result_tasks1 == target_tasks1);

  const std::string user_input2 = "rm T16";
  const auto [flag2, buffr2, session2] = interpreter.parse(user_input2);
  const std::string result_tasks2 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string target_tasks2 =
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
  CHECK(flag2 == "Invalid arguments");
  CHECK(result_tasks2 == target_tasks2);

  const std::string user_input3 = "rm";
  const auto [flag3, buffr3, session3] = interpreter.parse(user_input3);
  const std::string result_tasks3 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string target_tasks3 =
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
  CHECK(flag3 == "No arguments provided");
  CHECK(result_tasks3 == target_tasks3);

  const std::string user_input4 = "rm T5 -a";
  const auto [flag4, buffr4, session4] = interpreter.parse(user_input4);
  const std::string result_tasks4 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string target_tasks4 =
      "1 \"T1\" 0 46000 0 2 0\n"
      "2 \"T2\" 1 45891 0 0 0\n"
      "3 \"T3\" 1 46401 0 0 0\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 0 2 0\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 0 1 0\n"
      "11 \"T11\" 1 -1 0 0 0\n";
  CHECK(flag4 == "Removed task");
  CHECK(result_tasks4 == target_tasks4);
}

TEST_CASE("Removing category, but not tasks") {
  INTERPRETER_RM::reset_tasks_db();
  INTERPRETER_RM::reset_categories_db();
  INTERPRETER_RM::reset_relations_db();
  INTERPRETER_RM::reset_tracker();
  INTERPRETER_RM::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "rm @project";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string result_tasks1 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string target_tasks1 =
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
  const std::string result_categories1 =
      INTERPRETER_RM::get_file_content(PATH_CATEGORIES);
  const std::string target_categories1 =
      "0 \"Uncategorized\"\n"
      "1 \"Some category\"\n"
      "2 \"another_category\"\n";
  const std::string result_relations1 =
      INTERPRETER_RM::get_file_content(PATH_RELATIONS);
  const std::string target_relations1 =
      "0 2 1\n"
      "1 3 1\n"
      "2 5 2\n"
      "7 4 2\n"
      "9 11 1\n";
  CHECK(flag1 == "Removed category");
  CHECK(result_tasks1 == target_tasks1);
  CHECK(result_categories1 == target_categories1);
  CHECK(result_relations1 == target_relations1);
}

TEST_CASE("Removing category with tasks") {
  INTERPRETER_RM::reset_tasks_db();
  INTERPRETER_RM::reset_categories_db();
  INTERPRETER_RM::reset_relations_db();
  INTERPRETER_RM::reset_tracker();
  INTERPRETER_RM::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "rm @\"Some category\" -a";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string result_tasks1 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string target_tasks1 =
      "0 \"T0\" 0 45300 14 0 0\n"
      "1 \"T1\" 0 46000 0 2 0\n"
      "4 \"T4\" 1 45500 7 0 0\n"
      "5 \"T5\" 0 -1 0 0 0\n"
      "6 \"T6\" 2 -1 0 0 0\n"
      "7 \"T7\" 1 3401 0 2 0\n"
      "8 \"T8\" 2 3401 0 0 1\n"
      "9 \"T9\" 3 45300 0 0 0\n"
      "10 \"T10\" 2 45200 0 1 0\n";
  const std::string result_categories1 =
      INTERPRETER_RM::get_file_content(PATH_CATEGORIES);
  const std::string target_categories1 =
      "0 \"Uncategorized\"\n"
      "2 \"another_category\"\n"
      "3 \"project\"\n";
  const std::string result_relations1 =
      INTERPRETER_RM::get_file_content(PATH_RELATIONS);
  const std::string target_relations1 =
      "2 5 2\n"
      "3 6 3\n"
      "4 7 3\n"
      "6 8 3\n"
      "6 9 3\n"
      "7 4 2\n"
      "8 10 3\n";
  CHECK(flag1 == "Removed category with tasks");
  CHECK(result_tasks1 == target_tasks1);
  CHECK(result_categories1 == target_categories1);
  CHECK(result_relations1 == target_relations1);
}

TEST_CASE("Invalid prompts") {
  INTERPRETER_RM::reset_tasks_db();
  INTERPRETER_RM::reset_categories_db();
  INTERPRETER_RM::reset_relations_db();
  INTERPRETER_RM::reset_tracker();
  INTERPRETER_RM::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string target_tasks =
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
  const std::string target_categories =
      "0 \"Uncategorized\"\n"
      "1 \"Some category\"\n"
      "2 \"another_category\"\n"
      "3 \"project\"\n";
  const std::string target_relations =
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

  const std::string user_input1 = "rm @no_such_cat";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string result_tasks1 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string result_categories1 =
      INTERPRETER_RM::get_file_content(PATH_CATEGORIES);
  const std::string result_relations1 =
      INTERPRETER_RM::get_file_content(PATH_RELATIONS);
  CHECK(flag1 == "Invalid arguments");
  CHECK(result_tasks1 == target_tasks);
  CHECK(result_categories1 == target_categories);
  CHECK(result_relations1 == target_relations);

  const std::string user_input2 = "rm @no_such_cat -a";
  const auto [flag2, buffr2, session2] = interpreter.parse(user_input2);
  const std::string result_tasks2 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string result_categories2 =
      INTERPRETER_RM::get_file_content(PATH_CATEGORIES);
  const std::string result_relations2 =
      INTERPRETER_RM::get_file_content(PATH_RELATIONS);
  CHECK(flag2 == "Invalid arguments");
  CHECK(result_tasks2 == target_tasks);
  CHECK(result_categories2 == target_categories);
  CHECK(result_relations2 == target_relations);

  const std::string user_input3 = "rm no_such_task -a";
  const auto [flag3, buffr3, session3] = interpreter.parse(user_input3);
  const std::string result_tasks3 =
      INTERPRETER_RM::get_file_content(PATH_TASKS);
  const std::string result_categories3 =
      INTERPRETER_RM::get_file_content(PATH_CATEGORIES);
  const std::string result_relations3 =
      INTERPRETER_RM::get_file_content(PATH_RELATIONS);
  CHECK(flag3 == "Invalid arguments");
  CHECK(result_tasks3 == target_tasks);
  CHECK(result_categories3 == target_categories);
  CHECK(result_relations3 == target_relations);
}
