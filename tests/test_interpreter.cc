#include "../extern/include/catch.hpp"

#include "../include/syntax_analysis/interpreter.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define PATH_TASKS "../records/testing_records/bd/test_interpreter/tasks.mdb"
#define PATH_CATEGORIES                                                        \
  "../records/testing_records/bd/test_interpreter/categories.mdb"
#define PATH_RELATIONS                                                         \
  "../records/testing_records/bd/test_interpreter/relations.mdb"
#define PATH_TRACKER                                                           \
  "../records/testing_records/bd/test_interpreter/tracker.mcg"
#define PATH_SETTINGS                                                          \
  "../records/testing_records/bd/test_interpreter/settings.mcg"

namespace INTERPRETER_TEST_DB {
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

void reset_tracker() {
  const std::string content = "{last task id} {11}\n"
                              "{last category id} {3}\n"
                              "{last relation id} {9}\n";
  std::ofstream file(PATH_TRACKER);
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

} // namespace INTERPRETER_TEST_DB

TEST_CASE("Test command interpreter") {
  INTERPRETER_TEST_DB::reset_tasks_db();
  INTERPRETER_TEST_DB::reset_categories_db();
  INTERPRETER_TEST_DB::reset_relations_db();
  INTERPRETER_TEST_DB::reset_tracker();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "add my_task";
  interpreter.parse(user_input1);
  const std::string target1 = "0 \"T0\" 0 45300 14 0 0\n"
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
  const std::string tracker_target1 = "{last task id} {12}\n"
                                      "{last category id} {3}\n"
                                      "{last relation id} {9}\n";
  const std::string tracker_result1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TRACKER);
  CHECK(result1 == target1);
  CHECK(tracker_result1 == tracker_target1);
}
