#include "../../include/entry/entry_sorter_factory.hpp"

std::shared_ptr<EntrySorter> EntrySorterFactory::get(
    const std::string &sorter_str) noexcept {
  const auto it = sorter_map.find(sorter_str);
  if (it != sorter_map.end()) {
    return it->second;
  }
  return nullptr;
}

bool EntrySorterFactory::is_valid(
    const std::string &sorter_str) const noexcept {
  const auto it = sorter_map.find(sorter_str);
  return it != sorter_map.end();
}
