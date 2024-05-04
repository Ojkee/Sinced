#include "../../include/entry/entry_handler.hpp"

#include <algorithm>
#include <cstdlib>
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
  std::ifstream entry_file(path);
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

template <>
void EntryHandler::add_entry_to_db<EntryTask>(const EntryTask &entry) const {
  const std::string task_str = std::string(entry);
  append_to_db(task_str, tasks_db_path);
}

template <>
void EntryHandler::add_entry_to_db<EntryCategory>(
    const EntryCategory &entry) const {
  const std::string category_str = std::string(entry);
  append_to_db(category_str, categories_db_path);
}

template <>
void EntryHandler::add_entry_to_db<EntryRelation>(
    const EntryRelation &entry) const {
  const std::string relation_str = std::string(entry);
  append_to_db(relation_str, relations_db_path);
}

template <>
int8_t EntryHandler::replace_entry<EntryTask>(const EntryTask &old_entry,
                                              const EntryTask &new_entry) {
  return replace_entry_in_db(old_entry, new_entry, tasks_db_path);
}

template <>
int8_t
EntryHandler::replace_entry<EntryCategory>(const EntryCategory &old_entry,
                                           const EntryCategory &new_entry) {
  return replace_entry_in_db(old_entry, new_entry, categories_db_path);
}

template <>
int8_t
EntryHandler::replace_entry<EntryRelation>(const EntryRelation &old_entry,
                                           const EntryRelation &new_entry) {
  return replace_entry_in_db(old_entry, new_entry, relations_db_path);
}

template <>
std::shared_ptr<EntryTask>
EntryHandler::get_entry_by_content(const std::string &content) const {
  return entry_by_content<EntryTask>(content, tasks_db_path);
}

template <>
std::shared_ptr<EntryCategory>
EntryHandler::get_entry_by_content(const std::string &content) const {
  return entry_by_content<EntryCategory>(content, categories_db_path);
}

template <>
std::shared_ptr<EntryRelation>
EntryHandler::get_entry_by_content(const std::string &content) const {
  return entry_by_content<EntryRelation>(content, relations_db_path);
}

std::shared_ptr<EntryRelation>
EntryHandler::get_relation_by_ids(const std::string &task_content,
                                  const std::string category_content) const {
  std::ifstream relation_db(relations_db_path);
  if (!relation_db.is_open()) {
    std::cerr << "Couldn't open file: " << relations_db_path << "\n";
  }
  std::string line;
  while (getline(relation_db, line)) {
    std::shared_ptr<EntryRelation> entry =
        std::make_shared<EntryRelation>(line);
    if (entry->get_content() == task_content &&
        entry->get_content_category() == category_content) {
      return entry;
    }
  }
  return nullptr;
}

void EntryHandler::clear_db() {
  std::ofstream task_file, categories_file, relations_file;
  task_file.open(tasks_db_path, std::ofstream::out | std::ofstream::trunc);
  categories_file.open(categories_db_path,
                       std::ofstream::out | std::ofstream::trunc);
  relations_file.open(relations_db_path,
                      std::ofstream::out | std::ofstream::trunc);
  task_file.close();
  categories_file.close();
  relations_file.close();
  tasks.clear();
  categories.clear();
  relations.clear();
}

std::string EntryHandler::filtered_tasks_info() {
  const auto filtered_tasks = filter_load_db<EntryTask>(tasks_db_path);
  return entries_info<EntryTask>(filtered_tasks);
}

std::string EntryHandler::sorted_tasks_info() {
  const auto sorted_tasks = sorter->arranged(tasks);
  return entries_info(sorted_tasks);
}

template <typename EntryType>
std::vector<std::shared_ptr<EntryType>>
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

void EntryHandler::append_to_db(const std::string &entry_str,
                                const std::string &path) const {
  std::fstream entry_file;
  entry_file.open(path, std::ios_base::app);
  if (!entry_file.is_open()) {
    std::cerr << "Error loading file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  entry_file << entry_str << '\n';
}

template <typename EntryType>
int8_t EntryHandler::replace_entry_in_db(const EntryType &old_entry,
                                         const EntryType &new_entry,
                                         const std::string &path) const {
  std::ifstream file_in(path);
  if (!file_in.is_open()) {
    std::cerr << "Can't open file: " << path << "\n";
    exit(EXIT_FAILURE);
  }
  std::string line;
  std::stringstream buffr;
  bool found = false;
  while (getline(file_in, line)) {
    if (std::string(EntryType(line)) == std::string(old_entry)) {
      line = std::string(new_entry);
      found = true;
    }
    buffr << line << "\n";
  }
  file_in.close();
  std::ofstream file_out(path);
  if (!file_out.is_open()) {
    std::cerr << "Can't open file : " << path << "\n";
    exit(EXIT_FAILURE);
  }
  file_out << buffr.rdbuf();
  return found ? 0 : -1;
}

std::string EntryHandler::tasks_info_all() const {
  return entries_info<EntryTask>(tasks);
}

std::string EntryHandler::categories_info_all() const {
  return entries_info<EntryCategory>(categories);
}

template <typename EntryType>
std::string EntryHandler::entries_info(
    const std::vector<std::shared_ptr<EntryType>> &entries) const {
  std::ostringstream oss;
  std::for_each(entries.begin(), entries.end(),
                [&oss](const std::shared_ptr<EntryType> &entry) {
                  oss << entry->info() << '\n';
                });
  return oss.str();
}

std::string EntryHandler::task_info_by_id(const std::string &id) const {
  std::shared_ptr<EntryTask> task = entry_by_id<EntryTask>(id, tasks_db_path);
  return (task) ? task->info() : "No task with id: " + id;
}

std::string EntryHandler::task_info_by_id(const uint16_t &id) const {
  return task_info_by_id(std::to_string(id));
}

std::string EntryHandler::category_info_by_id(const std::string &id) const {
  std::shared_ptr<EntryCategory> category =
      entry_by_id<EntryCategory>(id, categories_db_path);
  return (category) ? category->info() : "No category with id: " + id;
}

std::string EntryHandler::category_info_by_id(const uint16_t &id) const {
  return category_info_by_id(std::to_string(id));
}

std::string EntryHandler::tasks_info_by_category_id(const std::string &id) {
  auto filtered_tasks = tasks_by_category_id(id);
  return (filtered_tasks.size() > 0) ? entries_info<EntryTask>(filtered_tasks)
                                     : "No tasks or category";
}

std::string EntryHandler::tasks_info_by_category_id(const uint16_t &id) {
  return tasks_info_by_category_id(std::to_string(id));
}

std::vector<std::shared_ptr<EntryTask>>
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

std::vector<std::shared_ptr<EntryTask>>
EntryHandler::tasks_by_category_id(const uint16_t &id) {
  return tasks_by_category_id(std::to_string(id));
}

template <typename EntryType>
std::shared_ptr<EntryType>
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

template <typename EntryType>
[[nodiscard]] std::shared_ptr<EntryType>
EntryHandler::entry_by_content(const std::string &content,
                               const std::string &path) const {
  std::ifstream data_base(path);
  if (!data_base.is_open()) {
    std::cerr << "Error loading file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  while (getline(data_base, line)) {
    std::shared_ptr<EntryType> entry = std::make_shared<EntryType>(line);
    if (entry->get_content() == content) {
      return entry;
    }
  }
  return nullptr;
}

template <typename EntryType>
std::vector<std::shared_ptr<EntryType>>
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
      filtered_entries.emplace_back(current_task);
    }
  }
  return filtered_entries;
}
