#ifndef FORMAT_DATE_HPP
#define FORMAT_DATE_HPP

#include <array>
#include <cstdint>
#include <string>

class FormatDate {
public:
  FormatDate() = default;
  FormatDate(std::string _separator) : separator(_separator) {}
  [[nodiscard]] virtual std::string
  get(const int16_t &day, const int16_t &month, const int16_t &year) const = 0;
  [[nodiscard]] virtual bool is_valid(const std::string &text) const = 0;
  [[nodiscard]] static std::string add_leading_char(const int16_t d,
                                                    uint16_t width, char ch);
  [[nodiscard]] std::tuple<int16_t, int16_t, int16_t>
  string_to_date_vars(std::string data_str) const;

  [[nodiscard]] virtual std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const = 0;
  void set_separator(std::string _separator) {
    separator = std::move(_separator);
  }
  virtual ~FormatDate() = default;

protected:
  std::string separator = "-";
  constexpr static std::array<std::string, 12> months_full_name = {
      "January", "February", "March",     "April",   "May",      "June",
      "July",    "August",   "September", "October", "November", "December"};
  constexpr static std::array<std::string, 12> roman_numerals = {
      "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII"};
  std::string build(const std::string &d1, const std::string &d2,
                    const std::string &d3) const;
  [[nodiscard]] std::string build_validation_expr(const std::string &d1,
                                                  const std::string &d2,
                                                  const std::string &d3) const;
};

class DDMMYYYY : public FormatDate {
public:
  DDMMYYYY() = default;
  DDMMYYYY(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class DDMMYY : public FormatDate {
public:
  DDMMYY() = default;
  DDMMYY(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class DMY : public FormatDate {
public:
  DMY() = default;
  DMY(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class MMDDYYYY : public FormatDate {
public:
  MMDDYYYY() = default;
  MMDDYYYY(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class MMDDYY : public FormatDate {
public:
  MMDDYY() = default;
  MMDDYY(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class MDY : public FormatDate {
public:
  MDY() = default;
  MDY(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class YYYYMMDD : public FormatDate {
public:
  YYYYMMDD() = default;
  YYYYMMDD(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class YYMMDD : public FormatDate {
public:
  YYMMDD() = default;
  YYMMDD(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid(const std::string &text) const override;
  std::tuple<int16_t, int16_t, int16_t>
  parse_from_string(const std::string &data_str) const override;
};

class MonthDY : public FormatDate {
public:
  MonthDY() = default;
  MonthDY(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid([[maybe_unused]] const std::string &text) const override {
    return false;
  }
  std::tuple<int16_t, int16_t, int16_t> parse_from_string(
      [[maybe_unused]] const std::string &data_str) const override {
    return {1, 1, 1970};
  };
};

class Roman : public FormatDate {
public:
  Roman() { separator = " "; };
  Roman(std::string _separator) : FormatDate(_separator) {}
  std::string get(const int16_t &day, const int16_t &month,
                  const int16_t &year) const override;
  bool is_valid([[maybe_unused]] const std::string &text) const override {
    return false;
  }
  std::tuple<int16_t, int16_t, int16_t> parse_from_string(
      [[maybe_unused]] const std::string &data_str) const override {
    return {1, 1, 1970};
  };
};

#endif // FORMAT_DATE_HPP
