#ifndef LEXER_HPP
#define LEXER_HPP

#include <algorithm>
#include <string>
#include <vector>

#include "token.hpp"

class Lexer {
public:
  Lexer() = default;
  std::vector<Token> tokenize(const std::string &line) const;

private:
  const TokenType get_token_type(const std::string &input_string) const;
  const std::string process_content(const std::string &content) const;
  const static bool is_number(const std::string &input_string) {
    return std::find_if(input_string.begin(), input_string.end(),
                        [](const auto &c) { return !std::isdigit(c); }) ==
           input_string.end();
  }
};
#endif // LEXER_HPP
