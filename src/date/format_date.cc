#include "../../include/date/format_date.hpp"

#include <cstddef>
#include <cstdint>
#include <format>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>

std::string FormatDate::add_leading_char(const int16_t d, uint16_t width,
                                         char ch) {
  std::ostringstream oss;
  oss << std::setw(width) << std::setfill(ch) << d;
  return oss.str();
}

std::tuple<int16_t, int16_t, int16_t> FormatDate::string_to_date_vars(
    std::string data_str) const {
  std::vector<int16_t> digits;
  std::regex numb_expr("(\\d+)");
  for (std::smatch m; std::regex_search(data_str, m, numb_expr);
       data_str = m.suffix()) {
    int16_t number = static_cast<int16_t>(std::stoi(m[1].str()));
    digits.emplace_back(number);
  }

  return {digits[0], digits[1], digits[2]};
}

std::string FormatDate::build(const std::string &d1, const std::string &d2,
                              const std::string &d3) const {
  std::ostringstream oss;
  oss << d1 << separator << d2 << separator << d3;
  return oss.str();
}

std::string FormatDate::build_validation_expr(const std::string &d1,
                                              const std::string &d2,
                                              const std::string &d3) const {
  return std::format("{}{}{}{}{}", d1, input_separators, d2, input_separators,
                     d3);
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
      build_validation_expr("\\d{1,2}", "\\d{1,2}", "\\d{4}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> DDMMYYYY::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d1, d2, d3};
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
      build_validation_expr("\\d{1,2}", "\\d{1,2}", "\\d{2}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> DDMMYY::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d1, d2, 2000 + d3};
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
      build_validation_expr("\\d{1,2}", "\\d{1,2}", "\\d{2}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> DMY::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d1, d2, 2000 + d3};
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
      build_validation_expr("\\d{1,2}", "\\d{1,2}", "\\d{4}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> MMDDYYYY::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d2, d1, d3};
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
      build_validation_expr("\\d{1,2}", "\\d{1,2}", "\\d{2}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> MMDDYY::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d2, d1, 2000 + d3};
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
      build_validation_expr("\\d{1,2}", "\\d{1,2}", "\\d{2}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> MDY::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d2, d1, 2000 + d3};
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
      build_validation_expr("\\d{4}", "\\d{1,2}", "\\d{1,2}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> YYYYMMDD::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d3, d2, d1};
}

std::string YYMMDD::get(const int16_t &day, const int16_t &month,
                        const int16_t &year) const {
  const std::string fday = FormatDate::add_leading_char(day, 2, '0');
  const std::string fmonth = FormatDate::add_leading_char(month, 2, '0');
  const std::string fyear = FormatDate::add_leading_char(year % 100, 2, '0');
  return build(fyear, fmonth, fday);
}

bool YYMMDD::is_valid(const std::string &text) const {
  const std::string expr =
      build_validation_expr("\\d{2}", "\\d{1,2}", "\\d{1,2}");
  return std::regex_match(text, std::regex(expr));
}

std::tuple<int16_t, int16_t, int16_t> YYMMDD::parse_from_string(
    const std::string &date_str) const {
  auto const [d1, d2, d3] = string_to_date_vars(date_str);
  return {d3, d2, 2000 + d1};
}

std::string MonthDY::get(const int16_t &day, const int16_t &month,
                         const int16_t &year) const {
  std::string fday = std::to_string(day);
  std::string fmonth = months_full_name.at(static_cast<std::size_t>(month - 1));
  std::string fyear = std::to_string(year);
  return std::format("{} {}, {}", fmonth, fday, fyear);
}

std::string Roman::get(const int16_t &day, const int16_t &month,
                       const int16_t &year) const {
  std::string fday = std::to_string(day);
  std::string fmonth = roman_numerals.at(static_cast<std::size_t>(month - 1));
  std::string fyear = std::to_string(year);
  return build(fday, fmonth, fyear);
}
