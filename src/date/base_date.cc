#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <numeric>

#include "../../include/date/base_date.hpp"

void BaseDate::add_days(int16_t _days) noexcept {
  int32_t days_since_begin = BaseDate::date_to_days(*this);
  *this = BaseDate::days_to_date(days_since_begin + _days);
  validate();
}

void BaseDate::add_months(int16_t _months) noexcept {
  if (_months == 0)
    return;
  month += (_months > 0) ? _months % 12 : -(-_months % 12);
  add_years(std::floor(_months / 12));
}

void BaseDate::add_years(int16_t _years) noexcept {
  year += _years;
  if (year < lower_bound_year)
    *this = BaseDate(1, 1, lower_bound_year);
  if (year > upper_bound_year)
    *this = BaseDate(31, 12, upper_bound_year);
  validate();
}

uint16_t BaseDate::day_of_week() noexcept {
  std::array<int16_t, 12> month_map{0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
  std::array<int16_t, 4> century_map{0, 6, 4, 2};

  int16_t YY = year % 100;
  int16_t year_code = (YY + (YY / 4));
  int16_t month_code = month_map.at(month - 1);
  int16_t century_code = century_map.at((year / 100 - 19) % 4);
  int16_t leap_code = static_cast<int16_t>((is_leap(year) && (month > 2)));
  return (year_code + month_code + century_code + day - leap_code) % 7;
}

std::string BaseDate::day_of_week_name() noexcept {
  return BaseDate::week_days.at(day_of_week());
}

int16_t BaseDate::remaining_days() noexcept {
  std::time_t now = std::time(NULL);
  std::tm *ltm = std::localtime(&now);
  BaseDate temp_date =
      BaseDate(ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
  uint16_t since_begin_to_now = BaseDate::date_to_days(temp_date);
  uint16_t since_begin_to_this = BaseDate::date_to_days(*this);
  return since_begin_to_this - since_begin_to_now;
}

uint16_t BaseDate::date_to_days(const BaseDate &bd) {
  auto begin = days_in_month.begin();
  auto end = days_in_month.begin() + bd.month - 1;
  int16_t accumulated_days = std::accumulate(begin, end, -lower_bound_day);
  if (BaseDate::is_leap(lower_bound_year)) {
    accumulated_days++;
  }
  if (bd.month >= 3 && BaseDate::is_leap(bd.year)) {
    accumulated_days++;
  }
  accumulated_days += bd.day;
  int32_t years_diff = bd.year - lower_bound_year;
  if (years_diff % 4 == 3) {
    ++accumulated_days;
  }
  accumulated_days +=
      years_diff * 365 + years_diff / 4 - years_diff / 100 + years_diff / 400;
  return accumulated_days;
}

BaseDate BaseDate::days_to_date(int32_t _days) {
  _days++;
  uint16_t yr = lower_bound_year;
  auto day_in_year = [](const uint16_t &y) {
    return 365 + static_cast<uint16_t>(BaseDate::is_leap(y));
  };
  while (_days > day_in_year(yr)) {
    _days -= day_in_year(yr);
    yr++;
  }
  auto days_in_current_month = [](const uint8_t &m, const uint16_t &y) {
    return days_in_month[m] +
           static_cast<uint16_t>(m == 1 && BaseDate::is_leap(y));
  };
  uint8_t mth{};
  while (_days > days_in_current_month(mth, yr)) {
    _days -= days_in_current_month(mth, yr);
    mth++;
  }
  return BaseDate(_days, mth + 1, yr);
}

BaseDate BaseDate::today() {
  std::time_t now = std::time(NULL);
  std::tm *ltm = std::localtime(&now);
  BaseDate result_date =
      BaseDate(ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
  return result_date;
}

void BaseDate::validate() {
  year = std::clamp((int)year, (int)lower_bound_year, (int)upper_bound_year);
  month = std::clamp((int)month, 1, 12);
  day = std::clamp((int)day, 1,
                   days_in_month[month - 1] + (month == 2 && is_leap(year)));
}
