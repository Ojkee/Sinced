#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "../extern/include/catch.hpp"
#include "../include/mcg_reader/reader.hpp"

#define TRACKER_PATH "../records/testing_records/mcgs/test_tracker.mcg"
#define SETTINGS_PATH "../records/testing_records/mcgs/test_settings.mcg"

namespace MCG_TESTING {

void reset_test_tracker() {
  std::ofstream file(TRACKER_PATH);
  const std::string contents =
      "<<{ field name } { value }>>\n\n"
      "{last task id} {0}\n"
      "{last category id} {49}\n"
      "{last relation id} {0}";
  file << contents;
}

void reset_test_settings() {
  std::ofstream file(SETTINGS_PATH);
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
}  // namespace MCG_TESTING

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

  const std::string target1 =
      "<<{ field name } { value }>>\n\n"
      "{last task id} {3}\n"
      "{last category id} {50}\n"
      "{last relation id} {2}\n";
  const std::string result1 = MCG_TESTING::get_content(TRACKER_PATH);
  CHECK(result1 == target1);
}

TEST_CASE("Testing Settings Date") {
  MCG_TESTING::reset_test_settings();
  SettingsHandler settings_handler = SettingsHandler(SETTINGS_PATH);

  std::shared_ptr<FormatDate> fd1 = settings_handler.get_format_date();
  const std::string r1 = (fd1) ? fd1->get(20, 4, 2069) : "nullptr";
  CHECK(r1 == "20-04-2069");

  const bool flag2 = settings_handler.set_format_date("YYYYMMDD", "--");
  std::shared_ptr<FormatDate> fd2 = settings_handler.get_format_date();
  const std::string r2 = (fd2) ? fd2->get(20, 4, 2069) : "nullptr";
  CHECK(flag2 == true);
  CHECK(r2 == "2069--04--20");

  const bool flag3 = settings_handler.set_format_date("YYYMDD", "");
  CHECK(flag3 == false);
}

TEST_CASE("Testing Settings Sorter") {
  MCG_TESTING::reset_test_settings();
  SettingsHandler settings_handler = SettingsHandler(SETTINGS_PATH);

  const bool flag1 = settings_handler.set_sorterer("deadline");
  const std::string r1 = MCG_TESTING::get_content(SETTINGS_PATH);
  const std::string t1 =
      "<<{ field name } { value }>>\n\n"
      "{date format} {DDMMYYYY}\n"
      "{date format separator} {-}\n"
      "{sort by} {deadline}\n";
  CHECK(flag1 == true);
  CHECK(r1 == t1);

  const bool flag2 = settings_handler.set_sorterer("deadlinee");
  CHECK(flag2 == false);
}

TEST_CASE("Testing reset to default settings") {
  MCG_TESTING::reset_test_settings();
  SettingsHandler settings_handler = SettingsHandler(SETTINGS_PATH);

  settings_handler.set_sorterer("deadline");
  settings_handler.set_format_date("MMDDYYYY", ".");
  const std::string changed =
      "<<{ field name } { value }>>\n\n"
      "{date format} {MMDDYYYY}\n"
      "{date format separator} {.}\n"
      "{sort by} {deadline}\n";
  const std::string changed_result = MCG_TESTING::get_content(SETTINGS_PATH);
  CHECK(changed_result == changed);
  settings_handler.reset_file();
  const std::string after_reset =
      "<<{ field name } { value }>>\n\n"
      "{date format} {DDMMYYYY}\n"
      "{date format separator} {-}\n"
      "{sort by} {default}\n";
  const std::string reset_result = MCG_TESTING::get_content(SETTINGS_PATH);
  CHECK(reset_result == after_reset);
}

TEST_CASE("Testing reset to default tracker") {
  MCG_TESTING::reset_test_tracker();
  TrackerHandler tracker_handler = TrackerHandler(TRACKER_PATH);

  for (uint8_t i = 0; i < 10; ++i) {
    tracker_handler.increment_field_value("last task id");
  }
  for (uint8_t i = 0; i < 6; ++i) {
    tracker_handler.increment_field_value("last relation id");
  }
  const std::string changed =
      "<<{ field name } { value }>>\n\n"
      "{last task id} {10}\n"
      "{last category id} {49}\n"
      "{last relation id} {6}\n";
  const std::string changed_result = MCG_TESTING::get_content(TRACKER_PATH);
  CHECK(changed_result == changed);
  tracker_handler.reset_file();
  const std::string reset_result = MCG_TESTING::get_content(TRACKER_PATH);
  const std::string after_reset =
      "<<{ field name } { value }>>\n\n"
      "{last task id} {0}\n"
      "{last category id} {0}\n"
      "{last relation id} {0}\n";
  CHECK(reset_result == after_reset);
}
