#include "../../include/syntax_analysis/interpreter.hpp"
#include "../../include/syntax_analysis/lexer.hpp"

#include <algorithm>
#include <format>
#include <memory>
#include <string>
#include <vector>

std::string Interpreter::parse(const std::string &user_input) {
  // TODO DateFormat getter from Settings file
  const std::vector<Token> tokens = Lexer::tokenize(user_input, DDMMYYYY());
  if (tokens.size() == 0) [[unlikely]] {
    return "No arguments";
  }

  if (tokens[0].type == TokenType::COMMAND) [[likely]] {
    // "add", "list", "rm", "set"
    if (tokens[0].content == "add") {
      return add_command(tokens);
    } else if (tokens[0].content == "rm") {
      // TODO more commads
    }
  }
  return "INTERPRETER::PARSE PLACEHOLDER RETURN VAL";
}

bool constexpr Interpreter::contains_token_type(
    const std::vector<Token> &tokens, const TokenType &token_type) {
  auto has_type = [&token_type](const Token t) { return t.type == token_type; };
  return std::find_if(tokens.begin(), tokens.end(), has_type) != tokens.end();
}

constexpr std::optional<std::string>
Interpreter::get_token_content_if_contains_type(
    const std::vector<Token> &tokens, const TokenType &token_type) {
  auto has_type = [&token_type](const Token t) { return t.type == token_type; };
  const auto token_iterator =
      std::find_if(tokens.begin(), tokens.end(), has_type);
  return (token_iterator == tokens.end())
             ? std::nullopt
             : std::optional(token_iterator->content);
}

void Interpreter::add_new_relation(const std::string &task_id,
                                   const std::string &category_id) {
  const std::string relation_token =
      std::format("{} {} {}", tracker_handler.next_id("last relation id"),
                  task_id, category_id);
  const EntryRelation relation = EntryRelation(relation_token);
  entry_handler.add_entry_to_db(relation);
  tracker_handler.increment_field_value("last relation id");
}

std::string Interpreter::add_command(const std::vector<Token> &tokens) {
  std::string returning_flag{"PLACEHOLDER ADD COMMAND"};
  if (tokens.size() <= 1) {
    return "No arguments provided";
  }
  return add_new_task_builder(tokens);
}

std::string
Interpreter::add_new_task_builder(const std::vector<Token> &tokens) {
  auto task_name =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::TEXT);
  auto category_name = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::CATEGORY_NAME);

  if (!task_name && !category_name) {
    return "Invalid arguments";
  }

  if (task_name && category_name) {
    auto category_ptr = entry_handler.get_entry_by_content<EntryCategory>(
        category_name.value());
    if (!category_ptr) {
      return std::format("No category: @\"{}\"", category_name.value());
    }

    const auto task_ptr =
        entry_handler.get_entry_by_content<EntryTask>(task_name.value());
    if (task_ptr) {
      const auto relation_ptr = entry_handler.get_relation_by_ids(
          task_ptr->get_id(), category_ptr->get_id());
      if (relation_ptr) {
        return std::format("\"{}\" already in @\"{}\"", task_name.value(),
                           category_name.value());
      }
      add_new_relation(task_ptr->get_id(), category_ptr->get_id());
      return std::format("Added: \"{}\" to @\"{}\"", task_name.value(),
                         category_name.value());
    }
    std::shared_ptr<EntryTask> new_task = build_task(tokens);
    entry_handler.add_entry_to_db(*new_task);
    add_new_relation(new_task->get_id(), category_ptr->get_id());
    return std::format("Added new task: \"{}\" to @\"{}\"", task_name.value(),
                       category_name.value());
  }

  if (!task_name && category_name) {
    auto category_ptr = entry_handler.get_entry_by_content<EntryCategory>(
        category_name.value());
    if (category_ptr) {
      return std::format("Category: @\"{}\" already exists",
                         category_name.value());
    }
    const std::string category_token =
        std::format("{} \"{}\"", tracker_handler.next_id("last category id"),
                    category_name.value());
    const auto category = EntryCategory(category_token);
    entry_handler.add_entry_to_db(category);
    tracker_handler.increment_field_value("last category id");
    return std::format("Added new category: @\"{}\"", category_name.value());
  }

  if (task_name && !category_name) {
    std::shared_ptr<EntryTask> new_task = build_task(tokens);
    entry_handler.add_entry_to_db(*new_task);
    return std::format("Added new task: \"{}\"", task_name.value());
  }

  return "PLACEHOLDER";
}

std::shared_ptr<EntryTask>
Interpreter::build_task(const std::vector<Token> &tokens) {
  EntryTask::Builder task_builder = EntryTask::Builder();
  task_builder.add_id(tracker_handler.next_id("last task id"));
  tracker_handler.increment_field_value("last task id");

  auto task_name =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::TEXT);
  task_builder.add_content(task_name.value());

  auto deadline_arg =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::DATE);
  auto options_arg = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::OPTION);
  auto params_arg = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::NUMBER);

  if (deadline_arg) {
    BaseDate deadline = BaseDate();
    deadline.set_formatter(std::make_unique<DDMMYYYY>());
    deadline.initialize_from_str(deadline_arg.value());
    task_builder.add_deadline(deadline);
    if (options_arg && options_arg.value().find("r") != std::string::npos) {
      int16_t days_{};
      int16_t months_{};
      int16_t years_{};
      if (options_arg->find("d") != std::string::npos) {
        days_ += 1;
      }
      if (options_arg->find("w") != std::string::npos) {
        days_ += 7;
      }
      if (options_arg->find("m") != std::string::npos) {
        months_ += 1;
      }
      if (options_arg->find("y") != std::string::npos) {
        years_ += 1;
      }
      if (params_arg) {
        int16_t param_val =
            static_cast<int16_t>(std::stoi(options_arg.value()));
        days_ *= param_val;
        months_ *= param_val;
        years_ *= param_val;
      }
      task_builder.add_recursive_days(static_cast<uint16_t>(days_));
      task_builder.add_recursive_months(static_cast<uint16_t>(months_));
      task_builder.add_recursive_years(static_cast<uint16_t>(years_));
    }
  } else if (!deadline_arg && options_arg) {
    int16_t days_{};
    int16_t months_{};
    int16_t years_{};
    if (options_arg->find("d") != std::string::npos) {
      days_ += 1;
    }
    if (options_arg->find("w") != std::string::npos) {
      days_ += 7;
    }
    if (options_arg->find("m") != std::string::npos) {
      months_ += 1;
    }
    if (options_arg->find("y") != std::string::npos) {
      years_ += 1;
    }
    if (params_arg) {
      int16_t param_val = static_cast<int16_t>(std::stoi(params_arg.value()));
      days_ *= param_val;
      months_ *= param_val;
      years_ *= param_val;
    }
    task_builder.add_deadline(BaseDate(days_, months_, years_));
    if (options_arg->find("r") != std::string::npos) {
      task_builder.add_recursive_days(static_cast<uint16_t>(days_));
      task_builder.add_recursive_months(static_cast<uint16_t>(months_));
      task_builder.add_recursive_years(static_cast<uint16_t>(years_));
    }
  }
  task_builder.add_status(Status::ongoing);
  return task_builder.get();
}
