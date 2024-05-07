#include "../../include/mcg_reader/reader.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>

std::optional<std::string> MCGFileHandler::get_value_by_field(
    const std::string &field_name) const {
  std::ifstream mcg_file(path);
  if (!mcg_file.is_open()) {
    std::cerr << "Can't open file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  while (getline(mcg_file, line)) {
    const auto [field, value] = parse_line(line);
    if (field == field_name) {
      return std::optional<std::string>(value);
    }
  }
  return std::nullopt;
}

int8_t MCGFileHandler::set_value_by_field(const std::string &field_name,
                                          const std::string &value) const {
  std::ifstream mcg_file_in(path);
  if (!mcg_file_in.is_open()) {
    std::cerr << "Can't open file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  bool found = false;
  std::stringstream buffr;
  while (getline(mcg_file_in, line)) {
    const auto [field, val] = parse_line(line);
    if (field == field_name) {
      line = "{" + field + "}" + " {" + value + "}";
      found = true;
    }
    buffr << line << "\n";
  }
  std::ofstream mcg_file_out(path);
  if (!mcg_file_in.is_open()) {
    std::cerr << "Can't open file2: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  mcg_file_out << buffr.rdbuf();
  return found ? 0 : -1;
}

std::tuple<std::string, std::string> MCGFileHandler::parse_line(
    const std::string &line) const {
  std::regex expr("\\{(.+?)\\} (.+)");
  std::smatch matches;
  if (std::regex_match(line, matches, expr)) {
    if (matches.size() == 3) {
      const std::string val =
          matches[2].str().substr(1, matches[2].str().size() - 2);
      return std::make_tuple(matches[1], val);
    }
  }
  return std::make_tuple(std::string(), std::string());
}

void TrackerHandler::increment_field_value(const std::string &field_name) {
  std::optional<std::string> val = get_value_by_field(field_name);
  if (val) {
    int32_t new_val = std::stoi(val.value()) + 1;
    set_value_by_field(field_name, std::to_string(new_val));
  }
}

std::string TrackerHandler::next_id(const std::string &field_name) {
  std::optional<std::string> next_val = get_value_by_field(field_name);
  if (next_val) {
    return std::to_string(std::stoi(next_val.value()) + 1);
  } else {
    return "0";
  }
}

std::shared_ptr<FormatDate> SettingsHandler::get_format_date() noexcept {
  std::ifstream settings_file(path);
  if (!settings_file.is_open()) {
    std::cerr << "Can't open file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  std::string separator{"-"};
  std::string format_date_str{"DDMMYYYY"};
  while (getline(settings_file, line)) {
    const auto [field, val] = parse_line(line);
    if (field == "date format") {
      format_date_str = val;
    } else if (field == "date format separator") {
      separator = val;
    }
  }
  return format_date_factory.get(format_date_str, separator);
}

bool SettingsHandler::set_format_date(const std::string &format_date_str,
                                      const std::string &separator) noexcept {
  if (format_date_factory.is_valid_format(format_date_str)) {
    set_value_by_field("date format", format_date_str);
    set_value_by_field("date format separator", separator);
    return true;
  }
  return false;
}

bool SettingsHandler::set_sorterer(const std::string &sorter_str) noexcept {
  if (entry_sorter_factory.is_valid(sorter_str)) {
    set_value_by_field("sort by", sorter_str);
    return true;
  }
  return false;
}
