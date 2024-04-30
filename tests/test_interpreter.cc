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

TEST_CASE("Test Add Command task CORRENT PROMPTS") {
  INTERPRETER_TEST_DB::reset_tasks_db();
  INTERPRETER_TEST_DB::reset_categories_db();
  INTERPRETER_TEST_DB::reset_relations_db();
  INTERPRETER_TEST_DB::reset_tracker();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "add my_task";
  const std::string flag1 = interpreter.parse(user_input1);
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
  CHECK(flag1 == "Added new task: \"my_task\"");
  CHECK(tracker_result1 == tracker_target1);

  const std::string user_input2 = "add @\"my category\"";
  std::string flag2 = interpreter.parse(user_input2);
  const std::string target2 = "0 \"Uncategorized\"\n"
                              "1 \"Some category\"\n"
                              "2 \"another_category\"\n"
                              "3 \"project\"\n"
                              "4 \"my category\"\n";
  const std::string result2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_CATEGORIES);
  const std::string tracker_target2 = "{last task id} {12}\n"
                                      "{last category id} {4}\n"
                                      "{last relation id} {9}\n";
  const std::string tracker_result2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TRACKER);
  CHECK(result2 == target2);
  CHECK(flag2 == "Added new category: @\"my category\"");
  CHECK(tracker_result2 == tracker_target2);

  const std::string user_input3 = "add new_task @project";
  const std::string flag3 = interpreter.parse(user_input3);
  const std::string result3_1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string target3_1 = "0 \"T0\" 0 45300 14 0 0\n"
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
  const std::string target3_2 = "0 2 1\n"
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
  const std::string tracker_target3 = "{last task id} {13}\n"
                                      "{last category id} {4}\n"
                                      "{last relation id} {10}\n";
  CHECK(result3_1 == target3_1);
  CHECK(result3_2 == target3_2);
  CHECK(flag3 == "Added new task: \"new_task\" to @\"project\"");
  CHECK(tracker_result3 == tracker_target3);

  const std::string user_input4 = "add taskerinho @categorinho";
  const std::string flag4 = interpreter.parse(user_input4);
  CHECK(flag4 == "No category: @\"categorinho\"");

  const std::string user_input5 = "add T4 @another_category";
  const std::string flag5 = interpreter.parse(user_input5);
  CHECK(flag5 == "\"T4\" already in @\"another_category\"");

  const std::string user_input6 = "add T4 @project";
  const std::string flag6 = interpreter.parse(user_input6);
  const std::string result6_1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string target6_1 = "0 \"T0\" 0 45300 14 0 0\n"
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
  const std::string target6_2 = "0 2 1\n"
                                "1 3 1\n"
                                "2 5 2\n"
                                "3 6 3\n"
                                "4 7 3\n"
                                "6 8 3\n"
                                "6 9 3\n"
                                "7 4 2\n"
                                "8 10 3\n"
                                "9 11 1\n"
                                "10 13 3\n"
                                "11 4 3\n";
  const std::string tracker_result6 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TRACKER);
  const std::string tracker_target6 = "{last task id} {13}\n"
                                      "{last category id} {4}\n"
                                      "{last relation id} {11}\n";
  CHECK(result6_1 == target6_1);
  CHECK(result6_2 == target6_2);
  CHECK(flag6 == "Added: \"T4\" to @\"project\"");
  CHECK(tracker_result6 == tracker_target6);
}

TEST_CASE("Test Add Command task with deadline") {
  INTERPRETER_TEST_DB::reset_tasks_db();
  INTERPRETER_TEST_DB::reset_categories_db();
  INTERPRETER_TEST_DB::reset_relations_db();
  INTERPRETER_TEST_DB::reset_tracker();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "add my_cool_task 20-04-2069";
  const std::string flag1 = interpreter.parse(user_input1);
  const std::string tasks_result1 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_target1 = "0 \"T0\" 0 45300 14 0 0\n"
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
  const std::string flag_target1 = "Added new task: \"my_cool_task\"";
  CHECK(flag1 == flag_target1);
  CHECK(tasks_result1 == tasks_target1);

  const std::string user_input2 = "add new_task 21-04-2069 -rw";
  const std::string flag2 = interpreter.parse(user_input2);
  const std::string tasks_result2 =
      INTERPRETER_TEST_DB::get_file_content(PATH_TASKS);
  const std::string tasks_target2 = "0 \"T0\" 0 45300 14 0 0\n"
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
  const std::string flag_target2 = "Added new task: \"new_task\"";
  CHECK(flag2 == flag_target2);
  CHECK(tasks_result2 == tasks_target2);
}
