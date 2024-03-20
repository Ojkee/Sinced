#include "../../include/entry/entry_sorter.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

SP_TASKS DeadlineSorter::arranged(SP_TASKS entries) const {
  std::erase_if(entries, [](const auto &entry) {
    return !entry || entry->get_deadline();
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

SP_TASKS AlphabeticalSorter::arranged(SP_TASKS entries) const {
  std::erase_if(entries, [](const auto &entry) { return !entry; });
  std::sort(entries.begin(), entries.end(),
            [](const auto &lhs, const auto &rhs) {
              return lhs->get_content() < rhs->get_content();
            });
  return entries;
}

// --TODO--
// [[nodiscard]] SP_TASKS CategoryWiseSorter::arranged(SP_RELATIONS relations,
//                                                     SP_TASKS entries) const {
//   std::erase_if(entries, [](const auto &entry) { return !entry; });
//   std::erase_if(relations, [](const auto &entry) { return !entry; });
//   std::sort(relations.begin(), relations.end(),
//             [](const auto &lhs, const auto &rhs) {
//               if (lhs->get_content_category() == rhs->get_content_category())
//               {
//                 return lhs->get_content() < rhs->get_content();
//               }
//               return lhs->get_content_category() <
//               rhs->get_content_category();
//             });
//   SP_TASKS result;
//   auto copy_task = [&](const std::shared_ptr<EntryRelation> &relation) {
//     auto place_it =
//         std::find_if(entries.begin(), entries.end(),
//                      [&](const std::shared_ptr<EntryTask> task) {
//                        return task->get_content() == relation->get_content();
//                      });
//     if (place_it != entries.end()) {
//       result.push_back(*place_it);
//     }
//   };
//   std::for_each(relations.begin(), relations.end(), copy_task);
//   return entries;
// }
