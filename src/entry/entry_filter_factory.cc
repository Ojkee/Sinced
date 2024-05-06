#include "../../include/entry/entry_filter_factory.hpp"

std::shared_ptr<EntryFilter> EntryFilterFactory::get(
    const std::string &filter_str) noexcept {
  const auto it = filter_map.find(filter_str);
  if (it != filter_map.end()) {
    return it->second;
  }
  return nullptr;
}

bool EntryFilterFactory::is_valid(
    const std::string &filter_str) const noexcept {
  const auto it = filter_map.find(filter_str);
  return it != filter_map.end();
}
