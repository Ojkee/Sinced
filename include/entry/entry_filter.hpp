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
  virtual void set_arg(const std::string &arg) const = 0;
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
  DefaultFilter() = default;
  void set_arg([[maybe_unused]] const std::string &arg) const override {}
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
  CategoryIDFilter() = default;
  CategoryIDFilter(const std::string &category_id_)
      : category_id(category_id_) {}
  void set_arg(const std::string &arg) const override {}
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
  std::string category_id{};
};

class DeadlineFilter : public EntryFilter {
public:
  DeadlineFilter() = default;
  DeadlineFilter(int16_t _day, int16_t _month, int16_t _year) {
    deadline_ = std::make_unique<BaseDate>(_day, _month, _year);
  }
  DeadlineFilter(const BaseDate &deadline)
      : deadline_(std::make_unique<BaseDate>(deadline)){};
  void set_arg(const std::string &arg) const override {}
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
  StatusFilter() = default;
  StatusFilter(const Status &status) : status_(status){};
  void set_arg(const std::string &arg) const override {}
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
