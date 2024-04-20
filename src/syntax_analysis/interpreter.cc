#include "../../include/syntax_analysis/interpreter.hpp"
#include "../../include/syntax_analysis/lexer.hpp"

#include <vector>

void Interpreter::parse(const std::string &user_input) {
  const std::vector<Token> tokens = Lexer::tokenize(user_input, DDMMYYYY());
  // "add", "list", "rm", "set"
  if (tokens.size() == 0) [[unlikely]] {
    return;
  }

  if (tokens[0].type == TokenType::COMMAND) [[likely]] {
    if (tokens[0].content == "add") {
      add_command(tokens);
    } else if (tokens[0].content == "rm") {
      // TODO
    }
  }
}

void Interpreter::add_command(const std::vector<Token> &tokens) {
  switch (tokens.size()) {
  case 2:
    if (tokens[1].type == TokenType::TEXT) {
      std::string next_id = tracker_handler.next_id("last task id");
      std::shared_ptr<EntryTask> new_entry = EntryTask::Builder()
                                                 .add_id(next_id)
                                                 .add_content(tokens[1].content)
                                                 .add_status(Status::ongoing)
                                                 .get();
      entry_handler.add_entry_to_db(*new_entry);
      tracker_handler.increment_field_value("last task id");
    }
    break;
  default:
    break;
  }
}
