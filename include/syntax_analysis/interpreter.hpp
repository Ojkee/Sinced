#ifndef INTERPTERE_HPP
#define INTERPTERE_HPP

#include "lexer.hpp"
#include "token.hpp"

#include <string>

class Interpreter {
public:
  Interpreter() = default;
  void parse(const std::string &user_input) const;

private:
  const Lexer lexer = Lexer();
};

#endif // INTERPTERE_HPP
