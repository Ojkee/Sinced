#include "../../include/entry/entry_sorter.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

SP_TASKS DeadlineSorter::arranged(SP_TASKS entries) const {
  std::erase_if(entries, [](const auto &entry) {
    return !entry || entry->get_deadline() == nullptr;
  });
  std::sort(entries.begin(), entries.end(),
            [](const auto &lhs, const auto &rhs) {
              return *lhs->get_deadline() < *rhs->get_deadline();
            });
  return entries;
}

SP_TASKS StatusSorter::arranged(SP_TASKS entries) const {
  std::erase_if(entries, [](const auto &entry) { return !entry; });
  std::sort(entries.begin(), entries.end(),
            [](const auto &lhs, const auto &rhs) {
              return static_cast<uint8_t>(lhs->get_status()) <
                     static_cast<uint8_t>(rhs->get_status());
            });
  return entries;
}
