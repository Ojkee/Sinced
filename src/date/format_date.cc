#include <format>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

#include "../../include/date/format_date.hpp"

std::string FormatDate::add_leading_char(const int16_t d, uint16_t width,
                                         char ch) {
  std::ostringstream oss;
  oss << std::setw(width) << std::setfill(ch) << d;
  return oss.str();
}

std::string FormatDate::build(const std::string &d1, const std::string &d2,
                              const std::string &d3) const {
  std::ostringstream oss;
  oss << d1 << separator << d2 << separator << d3;
  return oss.str();
}

std::string DDMMYYYY::get(const int16_t &day, const int16_t &month,
                          const int16_t &year) const {
  std::string fday = add_leading_char(day, 2, '0');
  std::string fmonth = add_leading_char(month, 2, '0');
  std::string fyear = add_leading_char(year, 4, '0');
  return build(fday, fmonth, fyear);
}

bool DDMMYYYY::is_valid(const std::string &text) const {
  const std::string expr =
      "\\d{1,2}" + separator + "\\d{1,2}" + separator + "\\d{4}";
  return std::regex_match(text, std::regex(expr));
}

std::string DDMMYY::get(const int16_t &day, const int16_t &month,
                        const int16_t &year) const {
  std::string fday = add_leading_char(day, 2, '0');
  std::string fmonth = add_leading_char(month, 2, '0');
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fday, fmonth, fyear);
}

bool DDMMYY::is_valid(const std::string &text) const {
  const std::string expr =
      "\\d{1,2}" + separator + "\\d{1,2}" + separator + "\\d{2}";
  return std::regex_match(text, std::regex(expr));
}

std::string DMY::get(const int16_t &day, const int16_t &month,
                     const int16_t &year) const {
  std::string fday = std::to_string(day);
  std::string fmonth = std::to_string(month);
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fday, fmonth, fyear);
}

bool DMY::is_valid(const std::string &text) const {
  const std::string expr =
      "\\d{1,2}" + separator + "\\d{1,2}" + separator + "\\d{2}";
  return std::regex_match(text, std::regex(expr));
}

std::string MMDDYYYY::get(const int16_t &day, const int16_t &month,
                          const int16_t &year) const {
  std::string fday = FormatDate::add_leading_char(day, 2, '0');
  std::string fmonth = FormatDate::add_leading_char(month, 2, '0');
  std::string fyear = FormatDate::add_leading_char(year, 4, '0');
  return build(fmonth, fday, fyear);
}

bool MMDDYYYY::is_valid(const std::string &text) const {
  const std::string expr =
      "\\d{1,2}" + separator + "\\d{1,2}" + separator + "\\d{4}";
  return std::regex_match(text, std::regex(expr));
}

std::string MMDDYY::get(const int16_t &day, const int16_t &month,
                        const int16_t &year) const {
  std::string fday = add_leading_char(day, 2, '0');
  std::string fmonth = add_leading_char(month, 2, '0');
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fmonth, fday, fyear);
}

bool MMDDYY::is_valid(const std::string &text) const {
  const std::string expr =
      "\\d{1,2}" + separator + "\\d{1,2}" + separator + "\\d{2}";
  return std::regex_match(text, std::regex(expr));
}

std::string MDY::get(const int16_t &day, const int16_t &month,
                     const int16_t &year) const {
  std::string fday = std::to_string(day);
  std::string fmonth = std::to_string(month);
  std::string fyear = add_leading_char(year % 100, 2, '0');
  return build(fmonth, fday, fyear);
}

bool MDY::is_valid(const std::string &text) const {
  const std::string expr =
      "\\d{1,2}" + separator + "\\d{1,2}" + separator + "\\d{2}";
  return std::regex_match(text, std::regex(expr));
}

std::string YYYYMMDD::get(const int16_t &day, const int16_t &month,
                          const int16_t &year) const {
  std::string fday = FormatDate::add_leading_char(day, 2, '0');
  std::string fmonth = FormatDate::add_leading_char(month, 2, '0');
  std::string fyear = FormatDate::add_leading_char(year, 4, '0');
  return build(fyear, fmonth, fday);
}

bool YYYYMMDD::is_valid(const std::string &text) const {
  const std::string expr =
      "\\d{4}" + separator + "\\d{1,2}" + separator + "\\d{1, 2}";
  return std::regex_match(text, std::regex(expr));
}

std::string MonthDY::get(const int16_t &day, const int16_t &month,
                         const int16_t &year) const {
  std::string fday = std::to_string(day);
  std::string fmonth = months_full_name.at(month - 1);
  std::string fyear = std::to_string(year);
  return std::format("{} {}, {}", fmonth, fday, fyear);
}

std::string Roman::get(const int16_t &day, const int16_t &month,
                       const int16_t &year) const {
  std::string fday = std::to_string(day);
  std::string fmonth = roman_numerals.at(month - 1);
  std::string fyear = std::to_string(year);
  return build(fday, fmonth, fyear);
}
