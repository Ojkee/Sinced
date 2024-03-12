#include "../../include/syntax_analysis/lexer.hpp"

#include <iomanip>
#include <sstream>

std::vector<Token> Lexer::tokenize(const std::string &line) const {
  std::vector<Token> tokens;
  std::istringstream iss(line);
  std::string word;
  while (iss >> std::quoted(word)) {
    const Token token{.type = get_token_type(word),
                      .content = process_content(word)};
    tokens.push_back(token);
  }
  return tokens;
}

const TokenType Lexer::get_token_type(const std::string &input_string) const {
  if (input_string.empty()) {
    return TokenType::BLANK_SPACE;
  }
  if (input_string.front() == '@') {
    return TokenType::CATEGORY_NAME;
  } else if (input_string.front() == '-') {
    return TokenType::OPTION;
  } else if (is_number(input_string)) {
    return TokenType::NUMBER;
  }
  return TokenType::BLANK_SPACE;
}

const std::string Lexer::process_content(const std::string &content) const {
  std::string result = content;

  return result;
}
