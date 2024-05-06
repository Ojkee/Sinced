#ifndef ENTRY_SORTERER_FACTORY_HPP_
#define ENTRY_SORTERER_FACTORY_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "./entry_sorter.hpp"

class EntrySorterFactory {
 public:
  EntrySorterFactory() = default;
  [[nodiscard]] std::shared_ptr<EntrySorter> get(
      const std::string &sorter_str) noexcept;
  bool is_valid(const std::string &sorter_str) const noexcept;

 private:
  const std::unordered_map<std::string, std::shared_ptr<EntrySorter>>
      sorter_map{{"default", std::make_shared<DefaultSorter>()},
                 {"deadline", std::make_shared<DeadlineSorter>()},
                 {"status", std::make_shared<StatusSorter>()},
                 {"alphabetical", std::make_shared<AlphabeticalSorter>()}};
};

#endif  // !ENTRY_SORTERER_FACTORY_HPP_
