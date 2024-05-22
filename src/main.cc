#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../include/syntax_analysis/interpreter.hpp"
#include "../include/syntax_analysis/lexer.hpp"

int main(int argc, char **argv) {
  std::vector<std::string> user_input;
  for (int i = 0; i < argc; ++i) {
    user_input.emplace_back(argv[i]);
    std::cout << user_input[static_cast<std::size_t>(i)] << " ";
  }
  const auto preprocessed_input = Lexer::preprocess(user_input, 1);

  Interpreter interpreter = Interpreter();
  const auto [flag, buffer, session] = interpreter.parse(preprocessed_input);
  std::cout << buffer;

  exit(EXIT_SUCCESS);
}
