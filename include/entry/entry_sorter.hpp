#ifndef ENTRY_SORTER_HPP
#define ENTRY_SORTER_HPP

#include <memory>
#include <vector>

#include "entry_base.hpp"

typedef std::vector<std::shared_ptr<EntryTask>> SP_TASKS;
typedef std::vector<std::shared_ptr<EntryRelation>> SP_RELATIONS;

class EntrySorter {
public:
  EntrySorter() = default;
  virtual ~EntrySorter() = default;
  virtual SP_TASKS arranged(SP_TASKS entries) const = 0;
};

class DefaultSorter : public EntrySorter {
public:
  DefaultSorter() = default;
  [[nodiscard]] SP_TASKS arranged(SP_TASKS entries) const override {
    return entries;
  }
};

class DeadlineSorter : public EntrySorter {
public:
  DeadlineSorter() = default;
  [[nodiscard]] SP_TASKS arranged(SP_TASKS entries) const override;
};

class StatusSorter : public EntrySorter {
public:
  StatusSorter() = default;
  [[nodiscard]] SP_TASKS arranged(SP_TASKS entries) const override;
};

class AlphabeticalSorter : public EntrySorter {
public:
  AlphabeticalSorter() = default;
  [[nodiscard]] SP_TASKS arranged(SP_TASKS entries) const override;
};

class CategoryWiseSorter : public EntrySorter {
public:
  CategoryWiseSorter() = default;
  [[nodiscard]] SP_TASKS arranged(SP_TASKS entries) = delete;
  [[nodiscard]] SP_TASKS arranged(const SP_RELATIONS &relations,
                                  SP_TASKS entries) const;
};

#endif // !ENTRY_SORTER_HPP
