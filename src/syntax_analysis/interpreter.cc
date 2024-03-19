#include "../../include/syntax_analysis/interpreter.hpp"
#include <vector>

void Interpreter::parse(const std::string &user_input) const {
  const std::vector<Token> tokens = lexer.tokenize(user_input);
}
