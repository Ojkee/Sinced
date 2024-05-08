// SETTING

#include <fstream>

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
  const auto [flag1, buffr1] = interpreter.parse(user_input1);
  const std::string result1 = INTERPRETER_SET::get_content(PATH_SETTINGS);
  const std::string target1 =
      "<<{ field name } { value }>>\n\n"
      "{date format} {YYYYMMDD}\n"
      "{date format separator} {-}\n"
      "{sort by} {default}\n";
  CHECK(flag1 == "Set \"date format\" to \"YYYYMMDD\"");
  CHECK(result1 == target1);

  const std::string user_input2 = "set -ds .";
  const auto [flag2, buffr2] = interpreter.parse(user_input2);
  const std::string result2 = INTERPRETER_SET::get_content(PATH_SETTINGS);
  const std::string target2 =
      "<<{ field name } { value }>>\n\n"
      "{date format} {YYYYMMDD}\n"
      "{date format separator} {.}\n"
      "{sort by} {default}\n";
  CHECK(flag2 == "Set \"date format separator\" to \".\"");
  CHECK(result2 == target2);
}
