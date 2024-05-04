#ifndef MCG_FILE_READER_HPP
#define MCG_FILE_READER_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>

class MCGFileHandler {
public:
  MCGFileHandler() = delete;
  MCGFileHandler(const std::string &path) : path(path){};
  virtual ~MCGFileHandler() = default;
  [[nodiscard]] std::optional<std::string>
  get_value_by_field(const std::string &field_name) const;
  // 0 IF VALUE WAS SET -1 IF FIELD NOT FOUND
  int8_t set_value_by_field(const std::string &field_name,
                            const std::string &value) const;

private:
  std::string path;

  [[nodiscard]] std::tuple<std::string, std::string>
  parse_line(const std::string &line) const;
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
};

#endif // !MCG_FILE_READER_HPP
