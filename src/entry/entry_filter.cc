
#include "../../include/entry/entry_filter.hpp"

bool CategoryIDFilter::passes(
    const std::shared_ptr<EntryRelation> &entry) const {
  if (!entry) {
    return false;
  }
  std::string content_category = entry->get_content_category();
  return content_category == category_id;
}

bool DeadlineFilter::passes(const std::shared_ptr<EntryTask> &entry) const {
  return (entry) ? entry->is_in_timeframe(deadline_) : false;
}
