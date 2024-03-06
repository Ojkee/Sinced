#include "../../include/entry/entry_handler.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

void EntryHandler::load_db() noexcept {
  if (!tasks_db_path.empty()) {
    tasks = load_entries<EntryTask>(tasks_db_path);
  }
  if (!categories_db_path.empty()) {
    categories = load_entries<EntryCategory>(categories_db_path);
  }
  if (!relations_db_path.empty()) {
    relations = load_entries<EntryRelation>(relations_db_path);
  }
}

template <typename EntryType>
std::vector<std::shared_ptr<EntryType>>
EntryHandler::filter_load_db(const std::string &path) noexcept {
  std::fstream entry_file(path);
  if (!entry_file.is_open()) {
    std::cerr << "Error loading file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  std::vector<std::shared_ptr<EntryType>> entries;
  while (getline(entry_file, line)) {
    std::shared_ptr<EntryType> entry = std::make_shared<EntryType>(line);
    if (filter->passes(entry)) {
      entries.push_back(entry);
    }
  }
  return entries;
}

[[nodiscard]] std::string EntryHandler::fitered_tasks_info() {
  const auto filtered_tasks = filter_load_db<EntryTask>(tasks_db_path);
  return entries_info<EntryTask>(filtered_tasks);
}

[[nodiscard]] std::string EntryHandler::sorted_tasks_info() {
  const auto sorted_tasks = sorter->arranged(tasks);
  return entries_info(sorted_tasks);
}

template <typename EntryType>
[[nodiscard]] std::vector<std::shared_ptr<EntryType>>
EntryHandler::load_entries(const std::string &file_name) const {
  std::ifstream entry_file(file_name);
  if (!entry_file.is_open()) {
    std::cerr << "Error loading file: "
              << ((file_name.empty()) ? "Empty name" : file_name) << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  std::vector<std::shared_ptr<EntryType>> entries;
  while (getline(entry_file, line)) {
    std::shared_ptr<EntryType> entry = std::make_shared<EntryType>(line);
    entries.push_back(entry);
  }
  return entries;
}

[[nodiscard]] std::string EntryHandler::tasks_info_all() const {
  return entries_info<EntryTask>(tasks);
}

[[nodiscard]] std::string EntryHandler::categories_info_all() const {
  return entries_info<EntryCategory>(categories);
}

template <typename EntryType>
[[nodiscard]] std::string EntryHandler::entries_info(
    const std::vector<std::shared_ptr<EntryType>> &entries) const {
  std::ostringstream oss;
  std::for_each(entries.begin(), entries.end(),
                [&oss](const std::shared_ptr<EntryType> &entry) {
                  oss << entry->info() << '\n';
                });
  return oss.str();
}

[[nodiscard]] std::string
EntryHandler::task_info_by_id(const std::string &id) const {
  std::shared_ptr<EntryTask> task = entry_by_id<EntryTask>(id, tasks_db_path);
  return (task) ? task->info() : "No task with id: " + id;
}

[[nodiscard]] std::string
EntryHandler::task_info_by_id(const uint16_t &id) const {
  return task_info_by_id(std::to_string(id));
}

[[nodiscard]] std::string
EntryHandler::category_info_by_id(const std::string &id) const {
  std::shared_ptr<EntryCategory> category =
      entry_by_id<EntryCategory>(id, categories_db_path);
  return (category) ? category->info() : "No category with id: " + id;
}

[[nodiscard]] std::string
EntryHandler::category_info_by_id(const uint16_t &id) const {
  return category_info_by_id(std::to_string(id));
}

[[nodiscard]] std::string
EntryHandler::tasks_info_by_category_id(const std::string &id) {
  auto filtered_tasks = tasks_by_category_id(id);
  return (filtered_tasks.size() > 0) ? entries_info<EntryTask>(filtered_tasks)
                                     : "No tasks or category";
}

[[nodiscard]] std::string
EntryHandler::tasks_info_by_category_id(const uint16_t &id) {
  return tasks_info_by_category_id(std::to_string(id));
}

template <typename EntryType>
[[nodiscard]] std::shared_ptr<EntryType>
EntryHandler::entry_by_id(const std::string &id,
                          const std::string &path) const {
  std::ifstream data_base(path);
  if (!data_base.is_open()) {
    std::cerr << "Error loading file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  while (getline(data_base, line)) {
    std::shared_ptr<EntryType> entry = std::make_shared<EntryType>(line);
    if (entry->get_id() == id) {
      return entry;
    }
  }
  return nullptr;
}

[[nodiscard]] std::vector<std::shared_ptr<EntryTask>>
EntryHandler::tasks_by_category_id(const std::string &id) {
  relations = load_entries<EntryRelation>(relations_db_path);
  set_filter(std::make_unique<CategoryIDFilter>(id));
  auto filtered_relations = filter->filtered(relations);
  std::set<std::string> tasks_ids;
  std::transform(
      filtered_relations.begin(), filtered_relations.end(),
      std::inserter(tasks_ids, tasks_ids.end()),
      [](const auto &entry_relation) { return entry_relation->get_content(); });
  return entry_if_id_in_set<EntryTask>(tasks_ids, tasks_db_path);
}

[[nodiscard]] std::vector<std::shared_ptr<EntryTask>>
EntryHandler::tasks_by_category_id(const uint16_t &id) {
  return tasks_by_category_id(std::to_string(id));
}

template <typename EntryType>
[[nodiscard]] std::vector<std::shared_ptr<EntryType>>
EntryHandler::entry_if_id_in_set(const std::set<std::string> &ids,
                                 const std::string &path) {
  std::vector<std::shared_ptr<EntryType>> filtered_entries;
  std::ifstream entries_db(path);
  if (!entries_db.is_open()) {
    std::cerr << "Cant open file: " << path << '\n';
  }
  std::string line;
  while (getline(entries_db, line)) {
    std::shared_ptr<EntryType> current_task = std::make_shared<EntryType>(line);
    std::string current_id = current_task->get_id();
    if (ids.contains(current_id)) {
      filtered_entries.push_back(current_task);
    }
  }
  return filtered_entries;
}
