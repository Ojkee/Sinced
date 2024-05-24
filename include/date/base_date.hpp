#ifndef BASE_DATE_HPP
#define BASE_DATE_HPP

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#include "format_date.hpp"

template <typename T>
concept Integral = std::is_integral_v<T>;

class BaseDate {
 public:
  constexpr static std::array<int16_t, 12> days_in_month{
      31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  constexpr static std::array<std::string, 7> week_days{
      "Sunday",   "Monday", "Tuesday", "Wednesday",
      "Thursday", "Friday", "Saturday"};

  BaseDate() = default;
  BaseDate(int16_t _day, int16_t _month, int16_t _year)
      : day(_day), month(_month), year(_year) {
    validate();
  }
  void initialize_from_str(const std::string &date_str);

  inline operator std::string() const noexcept {
    return formatter->get(day, month, year);
  }
  inline bool operator==(const BaseDate &other) const noexcept {
    return BaseDate::date_to_days(*this) == BaseDate::date_to_days(other);
  }
  inline bool operator>(const BaseDate &other) const noexcept {
    return BaseDate::date_to_days(*this) > BaseDate::date_to_days(other);
  }
  inline bool operator>=(const BaseDate &other) const noexcept {
    return *this > other || *this == other;
  }
  inline bool operator<(const BaseDate &other) const noexcept {
    return BaseDate::date_to_days(*this) < BaseDate::date_to_days(other);
  }
  inline bool operator<=(const BaseDate &other) const noexcept {
    return *this < other || *this == other;
  }
  static inline BaseDate shallow_copy(const BaseDate &bd) {
    return BaseDate::days_to_date(
        static_cast<int64_t>(BaseDate::date_to_days(bd)));
  }
  void add_days(int16_t _days) noexcept;
  void add_months(int16_t _months) noexcept;
  void add_years(int16_t _years) noexcept;
  uint16_t day_of_week() const noexcept;
  [[nodiscard]] std::string const day_of_week_name() const noexcept;
  [[nodiscard]] int16_t remaining_days() const noexcept;

  [[nodiscard]] static constexpr inline bool is_leap(
      const Integral auto &_year) noexcept {
    constexpr Integral auto FIRST_GREGORIAN = 1582;
    if (_year < FIRST_GREGORIAN) {
      return false;
    }
    const bool p = _year % 4 == 0;
    const bool q = _year % 100 == 0;
    const bool t = _year % 400 == 0;
    return (p && !q) || t;
  }
  [[nodiscard]] static uint32_t date_to_days(const BaseDate &bd);
  [[nodiscard]] static BaseDate days_to_date(int64_t _days);
  [[nodiscard]] static BaseDate today();
  [[nodiscard]] static BaseDate lower_bound_date() {
    return BaseDate(lower_bound_day, lower_bound_month, lower_bound_year);
  }
  [[nodiscard]] const FormatDate &get_formatter() const { return *formatter; }
  void set_formatter(std::shared_ptr<FormatDate> _formatter) {
    formatter = std::move(_formatter);
  }

 private:
  int16_t day{1};
  int16_t month{1};
  int16_t year{1970};
  void validate() noexcept;
  constexpr static uint16_t lower_bound_day{1};
  constexpr static uint16_t lower_bound_month{1};
  constexpr static uint16_t lower_bound_year{1970};
  constexpr static uint16_t upper_bound_year{9999};

  std::shared_ptr<FormatDate> formatter = std::make_shared<DDMMYYYY>();
};

#endif  // BASE_DATE_HPP
