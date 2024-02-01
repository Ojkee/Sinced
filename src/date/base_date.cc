#include "date/base_date.hpp"

#include <iomanip>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>
#include <ctime>


BaseDate::BaseDate() {}


BaseDate::BaseDate(int16_t _day, int16_t _month, int16_t _year)
  :day(_day), month(_month), year(_year)
{
  validate();
}


std::string BaseDate::to_str() noexcept
{
  return formatter->get(day, month, year);
}


void BaseDate::add_days(int16_t _days) noexcept {
  uint16_t days_since_begin = BaseDate::date_to_days(*this);
  uint16_t shifted_days = std::max(0, (int32_t)days_since_begin + _days);
  *this = BaseDate::days_to_date(shifted_days);
  validate();
}


void BaseDate::add_months(int16_t _months) noexcept {
  if (_months == 0) return;
  month += (_months > 0) ? _months % 12 : -(-_months % 12);
  add_years(std::floor(_months / 12));
}


void BaseDate::add_years(int16_t _years) noexcept {
  year += _years;
  if (year < lower_bound_year) *this = BaseDate(1, 1, lower_bound_year);
  if (year > upper_bound_year) *this = BaseDate(31, 12, upper_bound_year);
  validate();
}


uint16_t BaseDate::day_of_week() noexcept
{
  std::array<int16_t, 12> month_map{0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
  std::array<int16_t, 4> century_map{0, 6, 4, 2};

  int16_t YY = year % 100;
  int16_t year_code = (YY + (YY / 4));
  int16_t month_code = month_map.at(month - 1);
  int16_t century_code = century_map.at((year / 100 - 19) % 4);
  int16_t leap_code = (int16_t)(is_leap(year) && (month > 2));
  return (year_code + month_code + century_code + day - leap_code) % 7;
}


std::string BaseDate::day_of_week_name() noexcept
{
  return BaseDate::week_days.at(day_of_week());
}


int16_t BaseDate::remaining_days() noexcept
{
  std::time_t now =  std::time(NULL);
  std::tm *ltm = std::localtime(&now);
  BaseDate temp_date = BaseDate(
    ltm->tm_mday,
    ltm->tm_mon + 1,
    ltm->tm_year + 1900
  );
  uint16_t since_begin_to_now = BaseDate::date_to_days(temp_date);
  uint16_t since_begin_to_this = BaseDate::date_to_days(*this);
  return since_begin_to_this - since_begin_to_now;
}


uint16_t BaseDate::date_to_days(const BaseDate& bd)
{
  uint16_t year_days = (uint16_t)(((float)(bd.year) - (float)lower_bound_year) * 365.25);
  auto begin = days_in_month.begin();
  auto end = days_in_month.begin() + bd.month - 1;
  uint16_t month_days = std::accumulate(begin, end, bd.day);
  return year_days + month_days - 1;
}


BaseDate BaseDate::days_to_date(uint16_t _days)
{
  uint16_t year = (uint16_t)std::floor((float)_days / 365.25);
  _days -= std::floor((float)year * 365.25);
  uint16_t month{};
  for (; _days > days_in_month[month]; ++month) {
    _days -= days_in_month[month];
  }
  return BaseDate(_days + 1, month + 1, year + lower_bound_year);
}

BaseDate BaseDate::today()
{
  std::time_t now =  std::time(NULL);
  std::tm *ltm = std::localtime(&now);
  BaseDate result_date = BaseDate(
    ltm->tm_mday,
    ltm->tm_mon + 1,
    ltm->tm_year + 1900
  );
  return result_date;
}


void BaseDate::set_formatter(std::unique_ptr<FormatDate>&& _formatter)
{
  formatter = std::move(_formatter);
}


void BaseDate::validate()
{
  year = std::clamp((int)year, (int)lower_bound_year, (int)upper_bound_year);
  month = std::clamp((int)month, 1, 12);
  day = std::clamp((int)day, 1, days_in_month[month - 1] + (month == 2 && is_leap(year)));
}