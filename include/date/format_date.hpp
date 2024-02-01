#ifndef FORMAT_DATE_HPP
#define FORMAT_DATE_HPP


#include <string>
#include <cstdint>
#include <array>


class FormatDate {
public:
  FormatDate() {}
  FormatDate(std::string _separator) : separator(_separator) {}
  virtual ~FormatDate() = default;
  virtual std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const = 0;
  static std::string add_leading_char(const int16_t& d, uint16_t width, char ch);
  void set_separator(std::string _separator) { separator = std::move(_separator); }

protected:
  std::string separator = "-";
  std::array<std::string, 12> months_full_name = {"January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December" };
  std::array<std::string, 12> roman_numerals = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII" };
  std::string build(const std::string& d1, const std::string& d2, const std::string& d3) const;
};


class DDMMYYYY : public FormatDate {
public:
  DDMMYYYY() {};
  DDMMYYYY(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class DDMMYY : public FormatDate {
public:
  DDMMYY() {};
  DDMMYY(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class DMY : public FormatDate {
public:
  DMY() {};
  DMY(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class MMDDYYYY : public FormatDate {
public:
  MMDDYYYY() {};
  MMDDYYYY(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class MMDDYY : public FormatDate {
public:
  MMDDYY() {};
  MMDDYY(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class MDY : public FormatDate {
public:
  MDY() {};
  MDY(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class YYYYMMDD : public FormatDate {
public:
  YYYYMMDD() {};
  YYYYMMDD(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class MonthDY : public FormatDate {
public:
  MonthDY() {};
  MonthDY(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


class Roman : public FormatDate {
public:
  Roman() { separator = " "; };
  Roman(std::string _separator) : FormatDate(_separator) {};
  std::string get(const int16_t& day,const int16_t& month, const int16_t& year) const override;
};


#endif // FORMAT_DATE_HPP
