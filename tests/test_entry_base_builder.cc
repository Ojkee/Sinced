#include "../extern/include/catch.hpp"

#include "../include/entry/entry_base.hpp"
#include <string>

TEST_CASE("Building tasks using Builder pattern") {
  auto task1 = EntryTask::Builder().add_id("0").add_content("cool task").get();
  std::string t1 = "0 \"cool task\" 3 -1 0 0 0";
  CHECK(std::string(*task1) == t1);

  auto task2 = EntryTask::Builder()
                   .add_id("1")
                   .add_content("other task")
                   .add_status(Status::ongoing)
                   .get();
  std::string t2 = "1 \"other task\" 0 -1 0 0 0";
  CHECK(std::string(*task2) == t2);

  auto task3 = EntryTask::Builder()
                   .add_id("2")
                   .add_content("3rd task")
                   .add_status(Status::canceled)
                   .add_deadline(BaseDate(18, 11, 2000))
                   .add_recursive_years(1)
                   .get();
  std::string t3 = "2 \"3rd task\" 2 11279 0 0 1";
  CHECK(std::string(*task3) == t3);
}
