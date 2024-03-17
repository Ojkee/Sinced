#ifndef LEXER_HPP
#define LEXER_HPP

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "../../include/date/format_date.hpp"
#include "token.hpp"

class Lexer {
public:
  Lexer() = default;
  [[nodiscard]] std::vector<Token> tokenize(const std::string &line) const;
  [[nodiscard]] static constexpr bool
  is_unsigned_int(const std::string &input_string) {
    return std::find_if(input_string.begin(), input_string.end(),
                        [](const auto &c) { return !std::isdigit(c); }) ==
           input_string.end();
  }
  [[nodiscard]] static constexpr bool
  is_command(const std::string &input_string) {
    return std::binary_search(commands_set.begin(), commands_set.end(),
                              input_string);
  }
  [[nodiscard]] bool is_date(const std::string &input_string) const {
    return date_formatter->is_valid(input_string);
  }
  void set_formatter(std::unique_ptr<FormatDate> &&_formatter) {
    date_formatter = std::move(_formatter);
  }

private:
  [[nodiscard]] TokenType get_token_type(const std::string &input_string) const;
  [[nodiscard]] std::string process_content(const std::string &content) const;

  constexpr static auto commands_set = {"add", "list", "rm",
                                        "set"}; // KEEP LEXICOGRAPHIC
  std::unique_ptr<FormatDate> date_formatter = std::make_unique<DDMMYYYY>();
};

#endif // LEXER_HPP
