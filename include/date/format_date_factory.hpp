#ifndef FORMAT_DATE_FACTORY_HPP_
#define FORMAT_DATE_FACTORY_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "./format_date.hpp"

class FormatDateFactory {
 public:
  FormatDateFactory() = default;
  [[nodiscard]] std::shared_ptr<FormatDate> get(
      const std::string &format_date_str,
      const std::string &separator) noexcept;
  bool is_valid_format(const std::string &format_date_str) const noexcept;

 private:
  const std::unordered_map<std::string, std::shared_ptr<FormatDate>>
      date_formats_map{{"DDMMYYYY", std::make_shared<DDMMYYYY>()},
                       {"DDMMYY", std::make_shared<DDMMYY>()},
                       {"DMY", std::make_shared<DMY>()},
                       {"MMDDYYYY", std::make_shared<MMDDYYYY>()},
                       {"MMDDYY", std::make_shared<MMDDYY>()},
                       {"MDY", std::make_shared<MDY>()},
                       {"YYYYMMDD", std::make_shared<YYYYMMDD>()},
                       {"YYMMDD", std::make_shared<YYMMDD>()},
                       {"MonthDY", std::make_shared<MonthDY>()},
                       {"Roman", std::make_shared<Roman>()}};
};

#endif  // !FORMAT_DATE_FACTORY_HPP_
