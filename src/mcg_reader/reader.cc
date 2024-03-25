#include "../../include/mcg_reader/reader.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <tuple>

std::optional<std::string>
MCGFileHandler::get_value_by_field(const std::string &field_name) const {
  std::ifstream mcg_file(path);
  if (!mcg_file.is_open()) {
    std::cerr << "Can't open file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;
  while (getline(mcg_file, line)) {
    const auto [field, value] = parse_line(line);
    if (field == field_name) {
      return value;
    }
  }
  return std::nullopt;
}

int8_t MCGFileHandler::set_value_by_field(const std::string &field_name,
                                          const std::string &value) const {
  std::fstream mcg_file(path);
  if (!mcg_file.is_open()) {
    std::cerr << "Can't open file: " << path << '\n';
    exit(EXIT_FAILURE);
  }
  std::string line;

  while (getline(mcg_file, line)) {
  }
  return -1;
}

std::tuple<std::string, std::string>
MCGFileHandler::parse_line(const std::string &line) const {
  std::regex expr("\\{(.+?)\\} (.+)");
  std::smatch matches;
  if (std::regex_match(line, matches, expr)) {
    if (matches.size() == 3) {
      const std::string val =
          matches[2].str().substr(1, matches[2].str().size() - 2);
      return std::make_tuple(matches[1], val);
    }
  }
  return std::make_tuple("", "");
}
