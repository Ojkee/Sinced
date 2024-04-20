#include "../extern/include/catch.hpp"

#include "../include/mcg_reader/reader.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#define TRACKER_PATH "../records/testing_records/mcgs/test_tracker.mcg"
#define SETTINGS_PATH "../records/testing_records/mcgs/test_settings.mcg"

namespace MCG_TESTING {

void reset_test_tracker() {
  std::ofstream file(TRACKER_PATH);
  const std::string contents = "<<{ field name } { value }>>\n\n"
                               "{last task id} {0}\n"
                               "{last category id} {49}\n"
                               "{last relation id} {0}";
  file << contents;
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
} // namespace MCG_TESTING

TEST_CASE("Testing reading info from tracker") {
  MCG_TESTING::reset_test_tracker();
  MCGFileHandler reader = MCGFileHandler(TRACKER_PATH);

  const auto r1 = reader.get_value_by_field("last task id");
  const std::string result1 = (r1) ? r1.value() : "No such field";
  const std::string target1 = "0";
  CHECK(result1 == target1);

  const auto r2 = reader.get_value_by_field("last category id");
  const std::string result2 = (r2) ? r2.value() : "No such field";
  const std::string target2 = "49";
  CHECK(result2 == target2);

  const auto r3 = reader.get_value_by_field("last {no such table} id");
  const std::string result3 = (r3) ? r3.value() : "No such field";
  const std::string target3 = "No such field";
  CHECK(result3 == target3);
}

TEST_CASE("Testing overriding tracker file") {
  MCG_TESTING::reset_test_tracker();
  MCGFileHandler handler = MCGFileHandler(TRACKER_PATH);

  auto flag1 = handler.set_value_by_field("last task id", "2");
  CHECK(flag1 == 0);
  const auto r1 = handler.get_value_by_field("last task id");
  const std::string result1 = r1 ? r1.value() : "No val";
  CHECK(result1 == "2");

  auto flag2 = handler.set_value_by_field("last {no such table} id", "2");
  CHECK(flag2 == -1);

  MCG_TESTING::reset_test_tracker();
}

TEST_CASE("Testing Tracker") {
  MCG_TESTING::reset_test_tracker();
  TrackerHandler tracker_handler = TrackerHandler(TRACKER_PATH);

  tracker_handler.increment_field_value("last task id");
  tracker_handler.increment_field_value("last task id");
  tracker_handler.increment_field_value("last task id");

  tracker_handler.increment_field_value("last relation id");
  tracker_handler.increment_field_value("last relation id");

  tracker_handler.increment_field_value("last category id");

  const std::string target1 = "<<{ field name } { value }>>\n\n"
                              "{last task id} {3}\n"
                              "{last category id} {50}\n"
                              "{last relation id} {2}\n";
  const std::string result1 = MCG_TESTING::get_content(TRACKER_PATH);
  CHECK(result1 == target1);
}
