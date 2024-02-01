#ifndef BASE_DATE_HPP
#define BASE_DATE_HPP

#include "format_date.hpp"

#include <string>
#include <array>
#include <cstdint>
#include <memory>


class BaseDate {
public:
  int16_t day   {   1};
  int16_t month {   1};
  int16_t year  {1900};
  constexpr static std::array<int16_t, 12>
    days_in_month{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  constexpr static std::array<std::string, 7>
    week_days{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  BaseDate();
  BaseDate(int16_t _day, int16_t _month, int16_t _year);

  std::string to_str() noexcept;
  void add_days(int16_t _days) noexcept;
  void add_months(int16_t _months) noexcept;
  void add_years(int16_t _years) noexcept;
  uint16_t day_of_week() noexcept;
  std::string day_of_week_name() noexcept;
  int16_t remaining_days() noexcept;

  static bool is_leap(int16_t _year) noexcept { return _year % 4 == 0; }
  static uint16_t date_to_days(const BaseDate& bd);
  static BaseDate days_to_date(uint16_t _days);
  static BaseDate today();

  void set_formatter(std::unique_ptr<FormatDate>&& _formatter);
private:
  void validate();
  constexpr static uint16_t lower_bound_year {1900};
  constexpr static uint16_t upper_bound_year {9999};

  std::unique_ptr<FormatDate> formatter = std::make_unique<DDMMYYYY>();
};


#endif // BASE_DATE_HPP