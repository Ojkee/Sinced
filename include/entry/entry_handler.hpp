#ifndef ENTRY_HANDLER_HPP
#define ENTRY_HANDLER_HPP

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "entry_base.hpp"
#include "entry_filter.hpp"
#include "entry_sorter.hpp"

typedef std::vector<std::shared_ptr<EntryTask>> SP_TASKS;
typedef std::vector<std::shared_ptr<EntryCategory>> SP_CATEGORIES;
typedef std::vector<std::shared_ptr<EntryRelation>> SP_RELATIONS;
#define SP_T(x) std::vector<std::shared_ptr<x>>

class EntryHandler {
public:
  EntryHandler()
      : EntryHandler("../records/database/tasks.mdb",
                     "../records/database/categories.mdb",
                     "../records/database/relations.mdb"){};
  EntryHandler(const std::string &tasks_db_path_,
               const std::string &categories_db_path_,
               const std::string &relations_db_path_)
      : tasks_db_path(tasks_db_path_), categories_db_path(categories_db_path_),
        relations_db_path(relations_db_path_){};

  void load_db() noexcept;
  template <typename EntryType>
  SP_T(EntryType)
  filter_load_db(const std::string &path) noexcept;

  [[nodiscard]] std::size_t inline number_of_tasks() const noexcept {
    return tasks.size();
  }
  [[nodiscard]] std::size_t inline number_of_categories() const noexcept {
    return categories.size();
  }
  [[nodiscard]] std::size_t inline number_of_relations() const noexcept {
    return relations.size();
  }

  template <typename EntryType>
  void add_entry_to_db([[maybe_unused]] const EntryType &entry) const {
    std::cerr << "Not valid type of entry!";
  }
  template <> void add_entry_to_db<EntryTask>(const EntryTask &entry) const;
  template <>
  void add_entry_to_db<EntryCategory>(const EntryCategory &entry) const;
  template <>
  void add_entry_to_db<EntryRelation>(const EntryRelation &entry) const;

  template <typename EntryType>
  [[nodiscard]] std::shared_ptr<EntryType>
  get_entry_by_content([[maybe_unused]] const std::string &content) const {
    std::cerr << "Not valid type of entry!";
  }
  template <>
  [[nodiscard]] std::shared_ptr<EntryTask>
  get_entry_by_content(const std::string &content) const;
  template <>
  [[nodiscard]] std::shared_ptr<EntryCategory>
  get_entry_by_content(const std::string &content) const;
  template <>
  [[nodiscard]] std::shared_ptr<EntryRelation>
  get_entry_by_content(const std::string &content) const;

  [[nodiscard]] std::shared_ptr<EntryRelation>
  get_relation_by_ids(const std::string &task_content,
                      const std::string category_content) const;

  template <typename EntryType>
  int8_t replace_entry([[maybe_unused]] const EntryType &old_entry,
                       [[maybe_unused]] const EntryType &new_entry) {
    std::cerr << "Not valid type of entry!\n";
    exit(EXIT_FAILURE);
  }
  template <>
  int8_t replace_entry<EntryTask>(const EntryTask &old_entry,
                                  const EntryTask &new_entry);
  template <>
  int8_t replace_entry<EntryCategory>(const EntryCategory &old_entry,
                                      const EntryCategory &new_entry);
  template <>
  int8_t replace_entry<EntryRelation>(const EntryRelation &old_entry,
                                      const EntryRelation &new_entry);

  void clear_db();

  template <typename EntryType>
  [[nodiscard]] std::string entries_info(const SP_T(EntryType) & entries) const;

  [[nodiscard]] std::string filtered_tasks_info();
  [[nodiscard]] std::string sorted_tasks_info();
  [[nodiscard]] std::string tasks_info_all() const;
  [[nodiscard]] std::string categories_info_all() const;
  [[nodiscard]] std::string task_info_by_id(const std::string &id) const;
  [[nodiscard]] std::string task_info_by_id(const uint16_t &id) const;
  [[nodiscard]] std::string category_info_by_id(const std::string &id) const;
  [[nodiscard]] std::string category_info_by_id(const uint16_t &id) const;
  [[nodiscard]] std::string tasks_info_by_category_id(const std::string &id);
  [[nodiscard]] std::string tasks_info_by_category_id(const uint16_t &id);

  void set_filter(std::unique_ptr<EntryFilter> &&_filter) {
    filter = std::move(_filter);
  }
  void set_sorter(std::unique_ptr<EntrySorter> &&_sorter) {
    sorter = std::move(_sorter);
  }

private:
  template <typename EntryType>
  [[nodiscard]] SP_T(EntryType)
      load_entries(const std::string &file_name) const;

  void append_to_db(const std::string &entry_str,
                    const std::string &path) const;

  template <typename EntryType>
  int8_t replace_entry_in_db(const EntryType &old_entry,
                             const EntryType &new_entry,
                             const std::string &path) const;

  [[nodiscard]] SP_TASKS tasks_by_category_id(const std::string &id);
  [[nodiscard]] SP_TASKS tasks_by_category_id(const uint16_t &id);

  template <typename EntryType>
  [[nodiscard]] std::shared_ptr<EntryType>
  entry_by_id(const std::string &id, const std::string &path) const;
  template <typename EntryType>
  [[nodiscard]] std::shared_ptr<EntryType>
  entry_by_content(const std::string &content, const std::string &path) const;

  template <typename EntryType>
  [[nodiscard]] SP_T(EntryType)
      entry_if_id_in_set(const std::set<std::string> &ids,
                         const std::string &path);

  const std::string tasks_db_path;
  const std::string categories_db_path;
  const std::string relations_db_path;

  SP_TASKS tasks;
  SP_CATEGORIES categories;
  SP_RELATIONS relations;

  std::unique_ptr<EntryFilter> filter = std::make_unique<DefaultFilter>();
  std::unique_ptr<EntrySorter> sorter = std::make_unique<DefaultSorter>();
};

#endif // ENTRY_HANDLER_HPP
