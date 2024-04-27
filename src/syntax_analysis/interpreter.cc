#include "../../include/syntax_analysis/interpreter.hpp"
#include "../../include/syntax_analysis/lexer.hpp"

#include <format>
#include <vector>

std::string Interpreter::parse(const std::string &user_input) {
  const std::vector<Token> tokens = Lexer::tokenize(user_input, DDMMYYYY());
  // "add", "list", "rm", "set"
  if (tokens.size() == 0) [[unlikely]] {
    return "No arguments";
  }

  if (tokens[0].type == TokenType::COMMAND) [[likely]] {
    if (tokens[0].content == "add") {
      return add_command(tokens);
    } else if (tokens[0].content == "rm") {
      // TODO
    }
  }
  return "INTERPRETER::PARSE PLACEHOLDER RETURN VAL";
}

std::string Interpreter::add_command(const std::vector<Token> &tokens) {
  std::string returning_flag{"PLACEHOLDER ADD COMMAND"};
  switch (tokens.size()) {
  case 1:
    returning_flag = "No arguments provided";
    break;
  case 2:
    if (tokens[1].type == TokenType::TEXT) {
      returning_flag = add_new_task(tokens);
    } else if (tokens[1].type == TokenType::CATEGORY_NAME) {
      returning_flag = add_new_category(tokens);
    }
    break;
  case 3:
    if (tokens[1].type == TokenType::TEXT &&
        tokens[2].type == TokenType::CATEGORY_NAME) {
      returning_flag = add_new_task_to_category(tokens);
    }
    break;
  default:
    returning_flag = "Command not implemented yet\n";
    break;
  }
  return returning_flag;
}

std::string Interpreter::add_new_task(const std::vector<Token> &tokens) {
  const std::string next_id = tracker_handler.next_id("last task id");
  const std::shared_ptr<EntryTask> new_entry =
      EntryTask::Builder()
          .add_id(next_id)
          .add_content(tokens[1].content)
          .add_status(Status::ongoing)
          .get();
  entry_handler.add_entry_to_db(*new_entry);
  tracker_handler.increment_field_value("last task id");
  return std::format("Added new task: \"{}\"", tokens[1].content);
}

std::string Interpreter::add_new_category(const std::vector<Token> &tokens) {
  const std::string category_token_constr =
      std::format("{} \"{}\"", tracker_handler.next_id("last category id"),
                  tokens[1].content);
  const EntryCategory new_category = EntryCategory(category_token_constr);
  entry_handler.add_entry_to_db(new_category);
  tracker_handler.increment_field_value("last category id");
  return std::format("Added new category @\"{}\"", tokens[1].content);
}

std::string
Interpreter::add_new_task_to_category(const std::vector<Token> &tokens) {
  const auto category_ptr =
      entry_handler.get_entry_by_content<EntryCategory>(tokens[2].content);
  const auto task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(tokens[1].content);
  if (!category_ptr) {
    return std::format("No category: @\"{}\"", tokens[2].content);
  } else if (!task_ptr) {
    const std::string next_task_id = tracker_handler.next_id("last task id");
    std::shared_ptr<EntryTask> new_entry = EntryTask::Builder()
                                               .add_id(next_task_id)
                                               .add_content(tokens[1].content)
                                               .add_status(Status::ongoing)
                                               .get();
    entry_handler.add_entry_to_db(*new_entry);
    tracker_handler.increment_field_value("last task id");
    const std::string next_relation_id =
        tracker_handler.next_id("last relation id");
    const std::string relation_token_constr = std::format(
        "{} {} {}", next_relation_id, next_task_id, category_ptr->get_id());
    EntryRelation new_relation = EntryRelation(relation_token_constr);
    entry_handler.add_entry_to_db(new_relation);
    tracker_handler.increment_field_value("last relation id");
    return std::format("Added \"{}\" to @\"{}\"", tokens[1].content,
                       tokens[2].content);
  }
  const auto relation_ptr = entry_handler.get_relation_by_ids(
      task_ptr->get_id(), category_ptr->get_id());
  if (!relation_ptr) {
    const std::string next_relation_id =
        tracker_handler.next_id("last relation id");
    const std::string relation_token_constr =
        std::format("{} {} {}", next_relation_id, task_ptr->get_id(),
                    category_ptr->get_id());
    const EntryRelation new_relation = EntryRelation(relation_token_constr);
    entry_handler.add_entry_to_db(new_relation);
    tracker_handler.increment_field_value("last relation id");
    return std::format("Added \"{}\" to @\"{}\"", tokens[1].content,
                       tokens[2].content);
  }
  return std::format("{} already in @\"{}\"", tokens[1].content,
                     tokens[2].content);
}
