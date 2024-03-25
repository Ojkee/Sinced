#include "../extern/include/catch.hpp"

#include "../include/mcg_reader/reader.hpp"

TEST_CASE("Testing tracker file") {
  const std::string tracker_file_path =
      "../records/testing_records/mcgs/test_tracker.mcg";
  MCGFileHandler reader = MCGFileHandler(tracker_file_path);

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
