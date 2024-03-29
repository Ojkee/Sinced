#ifndef MCG_FILE_READER_HPP
#define MCG_FILE_READER_HPP

#include <cstdint>
#include <optional>
#include <tuple>

#include <string>
class MCGFileHandler {
public:
  MCGFileHandler() = delete;
  MCGFileHandler(const std::string path) : path(path){};
  [[nodiscard]] std::optional<std::string>
  get_value_by_field(const std::string &field_name) const;
  int8_t set_value_by_field(const std::string &field_name,
                            const std::string &value)
      const; // 0 IF VALUE WAS SET -1 IF FIELD NOT FOUND

private:
  std::string path;

  [[nodiscard]] std::tuple<std::string, std::string>
  parse_line(const std::string &line) const;
};

#endif // !MCG_FILE_READER_HPP
