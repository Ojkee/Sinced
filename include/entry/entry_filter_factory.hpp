#ifndef ENTRY_FILTER_FACTORY_HPP_
#define ENTRY_FILTER_FACTORY_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "./entry_filter.hpp"

class EntryFilterFactory {
 public:
  EntryFilterFactory() = default;
  [[nodiscard]] std::shared_ptr<EntryFilter> get(
      const std::string &filter_str) noexcept;
  bool is_valid(const std::string &filter_str) const noexcept;

 private:
  const std::unordered_map<std::string, std::shared_ptr<EntryFilter>>
      filter_map{
          {"default", std::make_shared<DefaultFilter>()},
          {"deadline", std::make_shared<DeadlineFilter>()},
          {"category", std::make_shared<CategoryIDFilter>()},
          {"status", std::make_shared<StatusFilter>()},
      };
};

#endif  // !ENTRY_FILTER_FACTORY_HPP_
