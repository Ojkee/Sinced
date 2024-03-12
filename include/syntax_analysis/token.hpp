#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
  COMMAND,
  TEXT,
  CATEGORY_NAME,
  OPTION,
  NUMBER,
  BLANK_SPACE
};

struct Token {
  const TokenType type;
  const std::string content;
};

#endif // TOKEN_HPP
