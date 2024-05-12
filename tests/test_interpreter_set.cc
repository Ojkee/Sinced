// SETTING

#include <fstream>
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

namespace INTERPRETER_SET {
void reset_test_settings() {
  std::ofstream file(PATH_SETTINGS);
  const std::string content =
      "<<{ field name } { value }>>\n\n"
      "{date format} {DDMMYYYY}\n"
      "{date format separator} {-}\n"
      "{sort by} {default}\n";
  file << content;
}

[[nodiscard]] std::string get_content(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "No suck file: " << path << "\n";
    exit(EXIT_FAILURE);
  }
  std::stringstream ss;
  std::string line;
  while (getline(file, line)) {
    ss << line << "\n";
  }
  return ss.str();
}
}  // namespace INTERPRETER_SET

TEST_CASE("Setting in setting.mcg file via interpreter") {
  // {"df", "date format"}, {"ds", "date format separator"}, {"s", "sort by"}};
  INTERPRETER_SET::reset_test_settings();
  Interpreter interpreter = Interpreter(
      PATH_TASKS, PATH_CATEGORIES, PATH_RELATIONS, PATH_TRACKER, PATH_SETTINGS);

  const std::string user_input1 = "set -df YYYYMMDD";
  const auto [flag1, buffr1, session1] = interpreter.parse(user_input1);
  const std::string result1 = INTERPRETER_SET::get_content(PATH_SETTINGS);
  const std::string target1 =
      "<<{ field name } { value }>>\n\n"
      "{date format} {YYYYMMDD}\n"
      "{date format separator} {-}\n"
      "{sort by} {default}\n";
  CHECK(flag1 == "Set \"date format\" to \"YYYYMMDD\"");
  CHECK(result1 == target1);

  const std::string user_input2 = "set -ds .";
  const auto [flag2, buffr2, session2] = interpreter.parse(user_input2);
  const std::string result2 = INTERPRETER_SET::get_content(PATH_SETTINGS);
  const std::string target2 =
      "<<{ field name } { value }>>\n\n"
      "{date format} {YYYYMMDD}\n"
      "{date format separator} {.}\n"
      "{sort by} {default}\n";
  CHECK(flag2 == "Set \"date format separator\" to \".\"");
  CHECK(result2 == target2);

  const std::string user_input3 = "set -s deadline";
  const auto [flag3, buffr3, session3] = interpreter.parse(user_input3);
  const std::string result3 = INTERPRETER_SET::get_content(PATH_SETTINGS);
  const std::string target3 =
      "<<{ field name } { value }>>\n\n"
      "{date format} {YYYYMMDD}\n"
      "{date format separator} {.}\n"
      "{sort by} {deadline}\n";
  CHECK(flag3 == "Set \"sorter\" to \"deadline\"");
  CHECK(result3 == target3);
}

TEST_CASE("Reading from settings") {
  INTERPRETER_SET::reset_test_settings();
  SettingsHandler settings_handler = SettingsHandler(PATH_SETTINGS);

  const std::shared_ptr<FormatDate> fd1 = settings_handler.get_format_date();
  CHECK(fd1->get(20, 4, 2069) == "20-04-2069");

  settings_handler.set_format_date("MMDDYYYY", "JD");
  const std::shared_ptr<FormatDate> fd2 = settings_handler.get_format_date();
  CHECK(fd2->get(20, 4, 2069) == "04JD20JD2069");

  const auto sorter_ptr1 = settings_handler.get_value_by_field("sort by");
  const std::string result1 = sorter_ptr1 ? sorter_ptr1.value() : "nullptr";
  CHECK(result1 == "default");

  settings_handler.set_sorterer("deadline");
  const auto sorter_ptr2 = settings_handler.get_value_by_field("sort by");
  const std::string result2 = sorter_ptr2 ? sorter_ptr2.value() : "nullptr";
  CHECK(result2 == "deadline");
}
