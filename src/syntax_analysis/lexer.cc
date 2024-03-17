#include "../../include/syntax_analysis/lexer.hpp"

#include <cctype>
#include <string>

[[nodiscard]] std::vector<Token>
Lexer::tokenize(const std::string &line) const {
  std::string word;
  std::vector<Token> tokens;
  bool in_quotes = false;
  auto append_and_clear = [&tokens, this](std::string &word) {
    tokens.emplace_back(get_token_type(word), process_content(word));
    word.clear();
  };
  for (const char &c : line) {
    if (c == '\"') {
      in_quotes = !in_quotes;
      if (!in_quotes && !word.empty()) {
        append_and_clear(word);
      }
    } else if (std::isspace(c) && !in_quotes) {
      if (!word.empty()) {
        append_and_clear(word);
      }
    } else [[likely]] {
      word += c;
    }
  }
  if (!word.empty()) {
    tokens.emplace_back(Token{get_token_type(word), process_content(word)});
  }
  return tokens;
}

[[nodiscard]] TokenType
Lexer::get_token_type(const std::string &input_string) const {
  if (input_string.empty()) {
    return TokenType::BLANK_SPACE;
  }
  if (input_string.front() == '@') {
    return TokenType::CATEGORY_NAME;
  } else if (input_string.front() == '-') {
    return TokenType::OPTION;
  } else if (Lexer::is_unsigned_int(input_string)) {
    return TokenType::NUMBER;
  } else if (Lexer::is_command(input_string)) {
    return TokenType::COMMAND;
  } else if (Lexer::is_date(input_string)) {
    return TokenType::DATE;
  }
  return TokenType::TEXT;
}

[[nodiscard]] std::string
Lexer::process_content(const std::string &content) const {
  if (content.front() == '@' || content.front() == '-') {
    return content.substr(1);
  }
  return content;
}
