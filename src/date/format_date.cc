#include "date/format_date.hpp"

#include <sstream>
#include <iomanip>
#include <format>
#include <utility>


std::string FormatDate::add_leading_char(const int16_t& d, uint16_t width, char ch)
{
  std::ostringstream oss;
  oss << std::setw(width) << std::setfill(ch) << d;
  return oss.str();
}


std::string FormatDate::build(const std::string& d1, const std::string& d2, const std::string& d3) const
{
  std::ostringstream oss;
  oss << d1 << separator << d2 << separator << d3;
  return oss.str();
}


std::string DDMMYYYY::get(const int16_t& day, const int16_t& month, const int16_t& year) const
{
  std::string fday = add_leading_char(day, 2, '0');
  std::string fmonth = add_leading_char(month, 2, '0');
  std::string fyear = add_leading_char(year, 4, '0');
  return build(fday, fmonth, fyear);
}


std::string DDMMYY::get(const int16_t& day, const int16_t& month, const int16_t& year) const
{
  std::string fday = add_leading_char(day, 2, '0');
  std::string fmonth = add_leading_char(month, 2, '0');
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fday, fmonth, fyear);
}


std::string DMY::get(const int16_t& day, const int16_t& month, const int16_t& year) const
{
  std::string fday = std::to_string(day);
  std::string fmonth = std::to_string(month);
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fday, fmonth, fyear);
}


std::string MMDDYYYY::get(const int16_t& day,const int16_t& month, const int16_t& year) const
{
  std::string fday = FormatDate::add_leading_char(day, 2, '0');
  std::string fmonth = FormatDate::add_leading_char(month, 2, '0');
  std::string fyear = FormatDate::add_leading_char(year, 4, '0');
  return build(fmonth, fday, fyear);
}


std::string MMDDYY::get(const int16_t& day,const int16_t& month, const int16_t& year) const
{
  std::string fday = add_leading_char(day, 2, '0');
  std::string fmonth = add_leading_char(month, 2, '0');
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fmonth, fday, fyear);
}


std::string MDY::get(const int16_t& day,const int16_t& month, const int16_t& year) const
{
  std::string fday = std::to_string(day);
  std::string fmonth = std::to_string(month);
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fmonth, fday, fyear);
}


std::string YYYYMMDD::get(const int16_t& day,const int16_t& month, const int16_t& year) const
{
  std::string fday = FormatDate::add_leading_char(day, 2, '0');
  std::string fmonth = FormatDate::add_leading_char(month, 2, '0');
  std::string fyear = FormatDate::add_leading_char(year, 4, '0');
  return build(fyear, fmonth, fday);
}


std::string MonthDY::get(const int16_t& day,const int16_t& month, const int16_t& year) const
{
  std::string fday = std::to_string(day);
  std::string fmonth = months_full_name.at(month - 1);
  std::string fyear = std::to_string(year);
  return std::format("{} {}, {}", fmonth, fday, fyear);
}


std::string Roman::get(const int16_t& day,const int16_t& month, const int16_t& year) const
{
  std::string fday = std::to_string(day);
  std::string fmonth = roman_numerals.at(month - 1);
  std::string fyear = std::to_string(year);
  return build(fday, fmonth, fyear);
}
