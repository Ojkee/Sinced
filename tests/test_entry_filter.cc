#include <fstream>
#include <memory>
#include <string>

#include "../extern/include/catch.hpp"
#include "../include/entry/entry_handler.hpp"

TEST_CASE("Filtering by category_id") {
  std::string testing_dir = "../records/testing_records/bd/";
  std::string tasks_path = testing_dir + "tasks_test2.mdb";
  std::string categories_path = testing_dir + "categories_test2.mdb";
  std::string relation_path = testing_dir + "relations_test2.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, categories_path, relation_path);

  std::string r1_1 = et1.tasks_info_by_category_id(2);
  std::string r1_2 = et1.tasks_info_by_category_id("2");
  std::string t1 =
      "\" title\"\n"
      "\tDone\n"
      "\tdeadline: 29-07-2094\n"
      "\"asjkdbna \"\n"
      "\tOngoing\n"
      "\tdeadline: None\n";

  REQUIRE(r1_1 == t1);
  REQUIRE(r1_2 == t1);

  std::string r2_1 = et1.tasks_info_by_category_id(99);
  std::string r2_2 = et1.tasks_info_by_category_id("99");
  std::string t2 = "No tasks or category";
  REQUIRE(r2_1 == t2);
  REQUIRE(r2_2 == t2);
}

std::string generate_deadline_tasks();

TEST_CASE("Filtering by deadline") {
  std::string testing_dir = "../records/testing_records/bd/";
  std::string tasks_path = testing_dir + "tasks_test_deadlines.mdb";
  std::ofstream file(tasks_path);
  file << generate_deadline_tasks();
  file.close();
  EntryHandler eh1 = EntryHandler(tasks_path, "", "");
  BaseDate deadline_date = BaseDate(21, 2, 2026);

  std::string today_date = std::string(BaseDate::today());
  std::string target_info1 =
      "\"In no repetetive\"\n\tDone\n\tdeadline: 26-02-2025\n"
      "\"Deadline date no repetetive\"\n\tDone\n\tdeadline: 21-02-2026\n"
      "\"Before repetetive in\"\n\tOngoing\n\tdeadline: 03-06-2024 every: 50 "
      "day \n"
      "\"In repetetive\"\n\tOngoing\n\tdeadline: 26-02-2025 every: 7 day \n"
      "\"Deadline date repetetive\"\n\tOngoing\n\tdeadline: 21-02-2026 every: "
      "20 day \n"
      "\"Today no repetetive\"\n\tOngoing\n\tdeadline: " +
      today_date +
      "\n"
      "\"Today repetetive\"\n\tOngoing\n\tdeadline: " +
      today_date + " every: 1 month \n";
  eh1.set_filter(std::make_unique<DeadlineFilter>(21, 2, 2026));
  std::string result_info1 = eh1.filtered_tasks_info();
  REQUIRE(result_info1 == target_info1);
}

std::string generate_deadline_tasks() {
  std::string tasks{
      "0 \"Before no repetetive\" 1 19777 0 0 0\n"
      "1 \"In no repetetive\" 1 20145 0 0 0\n"
      "2 \"After no repetetive\" 1 20510 0 0 0\n"
      "3 \"No deadline\" 1 -1 0 0 0\n"
      "4 \"Deadline date no repetetive\" 1 20505 0 0 0\n"
      "5 \"Before repetetive not in\" 0 19777 0 0 3\n"
      "6 \"Before repetetive in\" 0 19777 50 0 0\n"
      "7 \"In repetetive\" 0 20145 7 0 0\n"
      "8 \"After repetetive\" 0 20511 0 2 0\n"
      "9 \"Deadline date repetetive\" 0 20505 20 0 0\n"};
  auto today_date = BaseDate::date_to_days(BaseDate::today());
  std::string today_no_repretetive =
      "10 \"Today no repetetive\" 0 " + std::to_string(today_date) + " 0 0 0\n";
  std::string today_repretetive =
      "11 \"Today repetetive\" 0 " + std::to_string(today_date) + " 0 1 0\n";
  return tasks + today_no_repretetive + today_repretetive;
}

TEST_CASE("Filtering by status") {
  std::string testing_dir = "../records/testing_records/bd/";
  std::string tasks_path = testing_dir + "tasks_test2.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, "", "");
  et1.set_filter(std::make_unique<StatusFilter>(Status::ongoing));
  std::string result_info1 = et1.filtered_tasks_info();
  std::string target_info1 =
      "\"Another task title?\"\n\tOngoing\n\tdeadline: 09-01-2094 every: 14 "
      "day \n\"some_title\"\n\tOngoing\n\tdeadline: 10-12-2095 every: 2 month "
      "\n\"asjkdbna \"\n\tOngoing\n\tdeadline: None\n";
  REQUIRE(result_info1 == target_info1);

  et1.set_filter(std::make_unique<StatusFilter>(Status::done));
  std::string result_info2 = et1.filtered_tasks_info();
  std::string target_info2 =
      "\"3nd title\"\n\tDone\n\tdeadline: 24-08-2095\n\"clever "
      "name\"\n\tDone\n\tdeadline: 15-01-2097\n\" title\"\n\tDone\n\tdeadline: "
      "29-07-2094\n\"some_other title\"\n\tDone\n\tdeadline: "
      "25-04-1979\n\"some_other title\"\n\tDone\n\tdeadline: None\n";
  REQUIRE(result_info2 == target_info2);
}
