#include "../../include/syntax_analysis/interpreter.hpp"
#include "../../include/syntax_analysis/lexer.hpp"

#include <vector>

void Interpreter::parse(const std::string &user_input) const {
  const std::vector<Token> tokens = Lexer::tokenize(user_input, DDMMYYYY());
  // "add", "list", "rm", "set"
}
