#include "../../include/entry/entry_filter.hpp"

bool CategoryIDFilter::passes(
    const std::shared_ptr<EntryRelation> &entry) const {
  if (!entry) {
    return false;
  }
  const std::string content_category = entry->get_content_category();
  return content_category == category_id;
}

bool DeadlineFilter::passes(const std::shared_ptr<EntryTask> &entry) const {
  return (entry && deadline_) ? entry->is_in_timeframe(*deadline_) : false;
}

bool StatusFilter::passes(const std::shared_ptr<EntryTask> &entry) const {
  return entry->get_status() == status_;
}
