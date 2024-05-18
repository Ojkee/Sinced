#include "../../include/date/base_date.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>

void BaseDate::initialize_from_str(const std::string &date_str) {
  if (!formatter->is_valid(date_str)) {
    return;
  }
  const auto [day_, month_, year_] = formatter->parse_from_string(date_str);
  day = day_;
  month = month_;
  year = year_;
  validate();
}

void BaseDate::add_days(int16_t _days) noexcept {
  int32_t days_since_begin =
      static_cast<int32_t>(BaseDate::date_to_days(*this));
  *this = BaseDate::days_to_date(days_since_begin + _days);
  validate();
}

void BaseDate::add_months(int16_t _months) noexcept {
  if (_months == 0) return;
  const int16_t mth =
      (_months > 0) ? (_months % 12) : -static_cast<int16_t>((-_months % 12));
  month += mth;
  add_years(static_cast<int16_t>(std::floor(_months / 12)));
}

void BaseDate::add_years(int16_t _years) noexcept {
  year += _years;
  if (year < lower_bound_year) *this = BaseDate(1, 1, lower_bound_year);
  if (year > upper_bound_year) *this = BaseDate(31, 12, upper_bound_year);
  validate();
}

uint16_t BaseDate::day_of_week() const noexcept {
  constexpr std::array<int16_t, 12> month_map{0, 3, 3, 6, 1, 4,
                                              6, 2, 5, 0, 3, 5};
  constexpr std::array<int16_t, 4> century_map{0, 6, 4, 2};
  const int16_t YY = year % 100;
  const int16_t year_code = (YY + (YY / 4));
  const int16_t month_code = month_map.at(static_cast<std::size_t>(month - 1));
  const int16_t century_code =
      century_map.at(static_cast<std::size_t>((year / 100 - 19) % 4));
  const int16_t leap_code =
      static_cast<int16_t>((is_leap(year) && (month > 2)));
  const uint16_t result = static_cast<uint16_t>(
      (year_code + month_code + century_code + day - leap_code) % 7);
  return result;
}

std::string const BaseDate::day_of_week_name() const noexcept {
  return BaseDate::week_days.at(day_of_week());
}

int16_t BaseDate::remaining_days() const noexcept {
  std::time_t now = std::time(NULL);
  std::tm *ltm = std::localtime(&now);
  BaseDate temp_date = BaseDate(static_cast<int16_t>(ltm->tm_mday),
                                static_cast<int16_t>(ltm->tm_mon + 1),
                                static_cast<int16_t>(ltm->tm_year + 1900));
  uint32_t since_begin_to_now = BaseDate::date_to_days(temp_date);
  uint32_t since_begin_to_this = BaseDate::date_to_days(*this);
  return static_cast<int16_t>(since_begin_to_this - since_begin_to_now);
}

uint32_t BaseDate::date_to_days(const BaseDate &bd) {
  uint32_t sum_months = static_cast<uint32_t>(std::accumulate(
      days_in_month.begin(), days_in_month.begin() + bd.month - 1, 0));
  std::vector<uint32_t> years_range(
      static_cast<std::size_t>(bd.year - lower_bound_year));
  std::iota(years_range.begin(), years_range.end(), lower_bound_year);
  auto is_leap_ = [](const auto y) { return BaseDate::is_leap(y); };
  uint32_t leap = static_cast<uint32_t>(
      std::count_if(years_range.begin(), years_range.end(), is_leap_));
  leap += (bd.month > 2 && BaseDate::is_leap(bd.year)) ? 1 : 0;
  return static_cast<uint32_t>((bd.year - lower_bound_year)) * 365 + leap +
         sum_months - 1 + static_cast<uint32_t>(bd.day);
}

BaseDate BaseDate::days_to_date(int32_t _days) {
  _days++;
  int16_t yr = lower_bound_year;
  auto day_in_year = [](const int16_t &y) {
    return (BaseDate::is_leap(y)) ? 366 : 365;
  };
  while (_days > day_in_year(yr)) {
    _days -= day_in_year(yr);
    yr++;
  }
  auto days_in_current_month = [](const uint8_t &m, const int16_t &y) {
    return (m == 1 && BaseDate::is_leap(y)) ? days_in_month[m] + 1
                                            : days_in_month[m];
  };
  uint8_t mth{};
  while (_days > days_in_current_month(mth, yr)) {
    _days -= days_in_current_month(mth, yr);
    mth++;
  }
  return BaseDate(static_cast<int16_t>(_days), mth + 1, yr);
}

BaseDate BaseDate::today() {
  std::time_t now = std::time(NULL);
  std::tm *ltm = std::localtime(&now);
  auto caster_to_16 = [](auto &val) {
    if (val >= std::numeric_limits<int16_t>::min() &&
        val <= std::numeric_limits<int16_t>::max()) {
      return static_cast<int16_t>(val);
    }
    return std::numeric_limits<int16_t>::max();
  };
  BaseDate result_date =
      BaseDate(caster_to_16(ltm->tm_mday), caster_to_16(ltm->tm_mon) + 1,
               caster_to_16(ltm->tm_year) + 1900);
  return result_date;
}

void BaseDate::validate() noexcept {
  year = std::clamp<int16_t>(year, lower_bound_year, upper_bound_year);
  month = std::clamp<int16_t>(month, 1, 12);
  const int16_t max_days_in_month =
      days_in_month.at(static_cast<std::size_t>(month - 1)) +
      (month == 2 && is_leap(year));
  day = std::clamp<int16_t>(day, 1, max_days_in_month);
}
