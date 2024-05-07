#ifndef MCG_FILE_READER_HPP
#define MCG_FILE_READER_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>

#include "../date/format_date_factory.hpp"
#include "../entry/entry_sorter_factory.hpp"

class MCGFileHandler {
 public:
  MCGFileHandler() = delete;
  MCGFileHandler(const std::string &path) : path(path){};
  virtual ~MCGFileHandler() = default;
  [[nodiscard]] std::optional<std::string> get_value_by_field(
      const std::string &field_name) const;
  // 0 IF VALUE WAS SET -1 IF FIELD NOT FOUND
  int8_t set_value_by_field(const std::string &field_name,
                            const std::string &value) const;

 protected:
  std::string path;

  [[nodiscard]] std::tuple<std::string, std::string> parse_line(
      const std::string &line) const;
};

class TrackerHandler : public MCGFileHandler {
 public:
  TrackerHandler() = delete;
  TrackerHandler(const std::string &path) : MCGFileHandler(path){};

  void increment_field_value(const std::string &field_name);
  [[nodiscard]] std::string next_id(const std::string &field_name);
};

class SettingsHandler : public MCGFileHandler {
 public:
  SettingsHandler() = delete;
  SettingsHandler(const std::string &path) : MCGFileHandler(path){};

  [[nodiscard]] std::shared_ptr<FormatDate> get_format_date() noexcept;
  bool set_format_date(const std::string &format_date_str,
                       const std::string &separator) noexcept;
  bool set_sorterer(const std::string &sorter_str) noexcept;

 private:
  FormatDateFactory format_date_factory = FormatDateFactory();
  EntrySorterFactory entry_sorter_factory = EntrySorterFactory();
};

#endif  // !MCG_FILE_READER_HPP
