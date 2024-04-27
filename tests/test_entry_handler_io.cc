#include "../extern/include/catch.hpp"

#include "../include/entry/entry_handler.hpp"

TEST_CASE("Adding to db") {
  const std::string testing_dir = "../records/testing_records/bd/test_io/";
  const std::string tasks_path = testing_dir + "tasks.mdb";
  const std::string categories_path = testing_dir + "categories.mdb";
  const std::string relation_path = testing_dir + "relations.mdb";
  EntryHandler eh1 = EntryHandler(tasks_path, categories_path, relation_path);
  eh1.clear_db();

  eh1.add_entry_to_db(EntryTask("0 Title 0 -1 0 0 0"));
  eh1.add_entry_to_db(EntryTask("1 Title 0 -1 0 0 0"));
  eh1.add_entry_to_db(EntryTask("2 Title 0 -1 0 0 0"));
  eh1.add_entry_to_db(EntryTask("3 Title 0 -1 0 0 0"));

  eh1.add_entry_to_db(EntryCategory("0 cat"));

  eh1.add_entry_to_db(EntryRelation("0 0 1"));

  eh1.load_db();
  CHECK(eh1.number_of_tasks() == 4);
  CHECK(eh1.number_of_categories() == 1);
  CHECK(eh1.number_of_relations() == 1);

  eh1.clear_db();
  CHECK(eh1.number_of_tasks() == 0);
  CHECK(eh1.number_of_categories() == 0);
  CHECK(eh1.number_of_relations() == 0);

  std::shared_ptr<EntryTask> task1 =
      EntryTask::Builder().add_id("1").add_content("Task").get();
  eh1.add_entry_to_db(*task1);
  eh1.load_db();
  CHECK(eh1.number_of_tasks() == 1);
  eh1.clear_db();
}
