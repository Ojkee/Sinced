#ifndef ENTRY_FILTER_HPP
#define ENTRY_FILTER_HPP

#include <algorithm>
#include <memory>
#include <vector>

#include "../date/base_date.hpp"
#include "entry_base.hpp"

class EntryFilter {
public:
  EntryFilter() = default;
  virtual ~EntryFilter() = default;
  virtual bool passes(const std::shared_ptr<EntryTask> &entry) const = 0;
  virtual bool passes(const std::shared_ptr<EntryCategory> &entry) const = 0;
  virtual bool passes(const std::shared_ptr<EntryRelation> &entry) const = 0;

  template <typename EntryType>
  std::vector<std::shared_ptr<EntryType>>
  filtered(const std::vector<std::shared_ptr<EntryType>> &entries) const {
    std::vector<std::shared_ptr<EntryType>> filtered_entries;
    std::copy_if(
        entries.begin(), entries.end(), std::back_inserter(filtered_entries),
        [this](const auto &entry_ptr) { return this->passes(entry_ptr); });
    return filtered_entries;
  }
};

class DefaultFilter : public EntryFilter {
public:
  DefaultFilter() {}
  bool passes(
      [[maybe_unused]] const std::shared_ptr<EntryTask> &entry) const override {
    return true;
  }
  bool passes([[maybe_unused]] const std::shared_ptr<EntryCategory> &entry)
      const override {
    return true;
  }
  bool passes([[maybe_unused]] const std::shared_ptr<EntryRelation> &entry)
      const override {
    return true;
  }
};

class CategoryIDFilter : public EntryFilter {
public:
  CategoryIDFilter() = delete;
  CategoryIDFilter(const std::string &category_id_)
      : category_id(category_id_) {}
  bool passes(
      [[maybe_unused]] const std::shared_ptr<EntryTask> &entry) const override {
    return false;
  }
  bool passes([[maybe_unused]] const std::shared_ptr<EntryCategory> &entry)
      const override {
    return false;
  }
  bool passes(const std::shared_ptr<EntryRelation> &entry) const override;

  std::vector<std::shared_ptr<EntryRelation>>
  filtered(const std::vector<std::shared_ptr<EntryRelation>> &entries) const {
    return EntryFilter::filtered(entries);
  }

private:
  std::string category_id{0};
};

class DeadlineFilter : public EntryFilter {
public:
  DeadlineFilter() = delete;
  DeadlineFilter(int16_t _day, int16_t _month, int16_t _year) {
    this->deadline_ = std::make_unique<BaseDate>(_day, _month, _year);
  }
  bool passes(const std::shared_ptr<EntryTask> &entry) const override;
  bool passes([[maybe_unused]] const std::shared_ptr<EntryCategory> &entry)
      const override {
    return false;
  }
  bool passes([[maybe_unused]] const std::shared_ptr<EntryRelation> &entry)
      const override {
    return false;
  }

private:
  std::unique_ptr<BaseDate> deadline_ = nullptr;
};

class StatusFilter : public EntryFilter {
public:
  StatusFilter() = delete;
  StatusFilter(Status status) : status_(status){};
  bool passes(const std::shared_ptr<EntryTask> &entry) const override;
  bool passes([[maybe_unused]] const std::shared_ptr<EntryCategory> &entry)
      const override {
    return false;
  }
  bool passes([[maybe_unused]] const std::shared_ptr<EntryRelation> &entry)
      const override {
    return false;
  }

private:
  Status status_ = Status::undetermined;
};

#endif // !ENTRY_FILTER_HPP
