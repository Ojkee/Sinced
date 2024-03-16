#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
  COMMAND,
  TEXT,
  CATEGORY_NAME,
  OPTION,
  NUMBER,
  DATE,
  BLANK_SPACE // Just in case
};

struct Token {
  const TokenType type;
  const std::string content;
  bool operator==(const Token &other) const = default;
};

#endif // TOKEN_HPP
