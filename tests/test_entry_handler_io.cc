#include "../extern/include/catch.hpp"

#include "../include/entry/entry_handler.hpp"

TEST_CASE("Adding to db") {
  std::string testing_dir = "../records/testing_records/bd/test_io/";
  std::string tasks_path = testing_dir + "tasks.mdb";
  std::string categories_path = testing_dir + "categories.mdb";
  std::string relation_path = testing_dir + "relations.mdb";
  EntryHandler et1 = EntryHandler(tasks_path, categories_path, relation_path);
  et1.clear_db();

  et1.add_entry_to_db(EntryTask("0 Title 0 -1 0 0 0"));
  et1.add_entry_to_db(EntryTask("1 Title 0 -1 0 0 0"));
  et1.add_entry_to_db(EntryTask("2 Title 0 -1 0 0 0"));
  et1.add_entry_to_db(EntryTask("3 Title 0 -1 0 0 0"));

  et1.load_db();
  CHECK(et1.number_of_tasks() == 4);
  et1.clear_db();
  CHECK(et1.number_of_tasks() == 0);
}
