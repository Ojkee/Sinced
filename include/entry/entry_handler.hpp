#ifndef ENTRY_HANDLER_HPP
#define ENTRY_HANDLER_HPP

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "entry_base.hpp"
#include "entry_filter.hpp"

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
  std::vector<std::shared_ptr<EntryType>>
  filter_load_db(const std::string &path) noexcept;

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
  [[nodiscard]] std::string fitered_tasks_info();

private:
  template <typename EntryType>
  [[nodiscard]] std::vector<std::shared_ptr<EntryType>>
  load_entries(const std::string &file_name) const;
  template <typename EntryType>
  [[nodiscard]] std::string
  entries_info(const std::vector<std::shared_ptr<EntryType>> &entries) const;
  template <typename EntryType>
  [[nodiscard]] std::shared_ptr<EntryType>
  entry_by_id(const std::string &id, const std::string &path) const;

  [[nodiscard]] std::vector<std::shared_ptr<EntryTask>>
  tasks_by_category_id(const std::string &id);
  [[nodiscard]] std::vector<std::shared_ptr<EntryTask>>
  tasks_by_category_id(const uint16_t &id);

  template <typename EntryType>
  [[nodiscard]] std::vector<std::shared_ptr<EntryType>>
  entry_if_id_in_set(const std::set<std::string> &ids, const std::string &path);

  const std::string tasks_db_path;
  const std::string categories_db_path;
  const std::string relations_db_path;

  std::vector<std::shared_ptr<EntryTask>> tasks;
  std::vector<std::shared_ptr<EntryCategory>> categories;
  std::vector<std::shared_ptr<EntryRelation>> relations;

  std::unique_ptr<EntryFilter> filter = std::make_unique<DefaultFilter>();
};

#endif // ENTRY_HANDLER_HPP
