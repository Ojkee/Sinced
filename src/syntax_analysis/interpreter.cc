#include "../../include/syntax_analysis/interpreter.hpp"

#include <algorithm>
#include <format>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "../../include/syntax_analysis/lexer.hpp"

Parsing_Data Interpreter::parse(const std::string &user_input) {
  const std::shared_ptr<FormatDate> fd_ptr = settings_handler.get_format_date();
  const std::vector<Token> tokens = Lexer::tokenize(user_input, *fd_ptr);

  if (tokens.size() == 0) [[unlikely]] {
    return {.flag = Flag_Messages::no_args()};
  }

  if (tokens[0].type == TokenType::COMMAND) [[likely]] {
    // "add", "log", "rm", "set"
    if (tokens[0].content == "add") {
      return add_command(tokens);
    } else if (tokens[0].content == "rm") {
      // TODO more commads
    } else if (tokens[0].content == "log") {
      auto sorter = settings_handler.get_sorter();
      entry_handler.set_sorter(sorter);
      return log_command(tokens);
    } else if (tokens[0].content == "set") {
      return set_command(tokens);
    } else if (tokens[0].content == "mod") {
      return mod_command(tokens);
    }
  }
  // return {.flag = Flag_Messages::bad_return(tokens[0].content)};
  return {.flag = Flag_Messages::bad_return("Interpreter::parse")};
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

Parsing_Data Interpreter::add_command(const std::vector<Token> &tokens) {
  if (tokens.size() <= 1) {
    return {.flag = Flag_Messages::no_args()};
  }

  auto task_name =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::TEXT);
  auto category_name = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::CATEGORY_NAME);
  if (!task_name && !category_name) {
    return {.flag = Flag_Messages::invalid_args()};
  }

  if (task_name && category_name) {
    auto category_ptr = entry_handler.get_entry_by_content<EntryCategory>(
        category_name.value());
    if (!category_ptr) {
      return {.flag = Flag_Messages::no_category(category_name.value())};
    }
    const auto task_ptr =
        entry_handler.get_entry_by_content<EntryTask>(task_name.value());
    if (task_ptr) {
      const auto relation_ptr = entry_handler.get_relation_by_ids(
          task_ptr->get_id(), category_ptr->get_id());
      if (relation_ptr) {
        return {.flag = std::format("\"{}\" already in @\"{}\"",
                                    task_name.value(), category_name.value())};
      }
      add_new_relation(task_ptr->get_id(), category_ptr->get_id());
      return {.flag = std::format("Added: \"{}\" to @\"{}\"", task_name.value(),
                                  category_name.value())};
    }
    std::shared_ptr<EntryTask> new_task = build_task(tokens);
    entry_handler.add_entry_to_db(*new_task);
    add_new_relation(new_task->get_id(), category_ptr->get_id());
    return {.flag = std::format("Added new task: \"{}\" to @\"{}\"",
                                task_name.value(), category_name.value())};
  }

  if (!task_name && category_name) {
    auto category_ptr = entry_handler.get_entry_by_content<EntryCategory>(
        category_name.value());
    if (category_ptr) {
      return {.flag = std::format("Category: @\"{}\" already exists",
                                  category_name.value())};
    }
    const std::string category_token =
        std::format("{} \"{}\"", tracker_handler.next_id("last category id"),
                    category_name.value());
    const auto category = EntryCategory(category_token);
    entry_handler.add_entry_to_db(category);
    tracker_handler.increment_field_value("last category id");
    return {.flag = std::format("Added new category: @\"{}\"",
                                category_name.value())};
  }

  if (task_name && !category_name) {
    std::shared_ptr<EntryTask> new_task = build_task(tokens);
    entry_handler.add_entry_to_db(*new_task);
    return {.flag = std::format("Added new task: \"{}\"", task_name.value())};
  }

  return {.flag =
              Flag_Messages::bad_return("Interpreter::add_new_task_builder")};
}

std::shared_ptr<EntryTask> Interpreter::build_task(
    const std::vector<Token> &tokens) {
  EntryTask::Builder task_builder = EntryTask::Builder();
  task_builder.add_id(tracker_handler.next_id("last task id"));
  tracker_handler.increment_field_value("last task id");
  task_builder.add_status(Status::ongoing);

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
    deadline.initialize_from_str(deadline_arg.value());
    task_builder.add_deadline(deadline);
    if (options_arg && options_arg.value().find("r") != std::string::npos) {
      uint16_t days_{};
      uint16_t months_{};
      uint16_t years_{};
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
        uint16_t param_val =
            static_cast<uint16_t>(std::stoi(params_arg.value()));
        days_ *= param_val;
        months_ *= param_val;
        years_ *= param_val;
      }
      task_builder.add_recursive_days(days_);
      task_builder.add_recursive_months(months_);
      task_builder.add_recursive_years(years_);
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
    BaseDate deadline_ = BaseDate::today();
    deadline_.add_days(days_);
    deadline_.add_months(months_);
    deadline_.add_years(years_);
    task_builder.add_deadline(deadline_);
    if (options_arg->find("r") != std::string::npos) {
      task_builder.add_recursive_days(static_cast<uint16_t>(days_));
      task_builder.add_recursive_months(static_cast<uint16_t>(months_));
      task_builder.add_recursive_years(static_cast<uint16_t>(years_));
    }
  }
  return task_builder.get();
}

Parsing_Data Interpreter::log_command(const std::vector<Token> &tokens) {
  if (tokens.size() <= 1) {
    return {.flag = Flag_Messages::no_args()};
  }

  const auto text_arg =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::TEXT);
  const auto category_name = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::CATEGORY_NAME);
  const auto options_arg = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::OPTION);

  if (!(text_arg || category_name || options_arg)) {
    return {.flag = Flag_Messages::invalid_args()};
  }

  if (text_arg && !options_arg) {
    std::shared_ptr<EntryTask> task_ptr =
        entry_handler.get_entry_by_content<EntryTask>(text_arg.value());
    if (task_ptr) {
      return {.flag = std::format("Logged: \"{}\"", text_arg.value()),
              .out_buffer = task_ptr->info()};
    }
    return {
        .flag = Flag_Messages::no_task(text_arg.value()),
        .out_buffer = std::format("No task named: \"{}\"", text_arg.value())};
  } else if (category_name) {
    std::shared_ptr<EntryCategory> category_ptr =
        entry_handler.get_entry_by_content<EntryCategory>(
            category_name.value());
    if (category_ptr) {
      const std::string tasks_in_category_info =
          entry_handler.tasks_info_by_category_id(category_ptr->get_id());
      return {.flag = std::format("Logged: @\"{}\"", category_name.value()),
              .out_buffer = tasks_in_category_info};
    }
    return {.flag = Flag_Messages::no_category(category_name.value()),
            .out_buffer = std::format("No category named: @\"{}\"",
                                      category_name.value())};
  } else if (options_arg) {
    if (options_arg.value() == "a") {
      entry_handler.load_filtered_tasks();
      const std::string all_tasks_info = entry_handler.sorted_tasks_info();
      return {.flag = "Logged all tasks", .out_buffer = all_tasks_info};
    }
    return parse_log_filter(options_arg.value(), tokens);
  }

  return {.flag = Flag_Messages::bad_return("Interpreter::log_command")};
}

Parsing_Data Interpreter::parse_log_filter(const std::string &option,
                                           const std::vector<Token> &tokens) {
  if (option == "deadline") {
    const auto date_arg = Interpreter::get_token_content_if_contains_type(
        tokens, TokenType::DATE);
    if (!date_arg) {
      return {.flag = Flag_Messages::invalid_args(),
              .out_buffer = "Invalid argument for filter: deadline"};
    }
    const auto fd = settings_handler.get_format_date();
    const auto [d, m, y] = fd->parse_from_string(date_arg.value());
    entry_handler.set_filter(
        std::make_shared<DeadlineFilter>(BaseDate(d, m, y)));
  } else if (option == "status") {
    const auto text_arg = Interpreter::get_token_content_if_contains_type(
        tokens, TokenType::TEXT);
    if (!text_arg) {
      return {.flag = Flag_Messages::invalid_args(),
              .out_buffer = "Invalid argument for filter: status"};
    }
    std::string lower_arg{};
    std::transform(text_arg.value().begin(), text_arg.value().end(),
                   std::back_inserter(lower_arg),
                   [](const auto c) { return std::tolower(c); });
    std::shared_ptr<StatusFilter> sf = std::make_shared<StatusFilter>();
    if (lower_arg == "ongoing") {
      sf->set_arg(Status::ongoing);
    } else if (lower_arg == "done") {
      sf->set_arg(Status::done);
    } else if (lower_arg == "canceled") {
      sf->set_arg(Status::canceled);
    } else {
      return {.flag = Flag_Messages::invalid_args(),
              .out_buffer = std::format("No status \"{}\"", text_arg.value())};
    }
    entry_handler.set_filter(sf);
  } else {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = std::format("No filter named: {}", option)};
  }
  entry_handler.load_filtered_tasks();
  const std::string filtered_tasks_info = entry_handler.sorted_tasks_info();
  return {.flag = "Logged filtered tasks", .out_buffer = filtered_tasks_info};
}

Parsing_Data Interpreter::set_command(const std::vector<Token> &tokens) {
  if (tokens.size() <= 1) {
    return {.flag = Flag_Messages::no_args()};
  }

  const auto setting_field_arg =
      Interpreter::get_token_content_if_contains_type(tokens,
                                                      TokenType::OPTION);
  const auto new_setting_arg =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::TEXT);

  if (!setting_field_arg || !new_setting_arg) {
    return {.flag = Flag_Messages::invalid_args()};
  }

  const auto it = set_params.find(setting_field_arg.value());
  if (it == set_params.end()) {
    return {.flag = Flag_Messages::invalid_args()};
  }

  if (it->second == "date format") {
    const auto sep =
        settings_handler.get_value_by_field("date format separator");
    const bool set_flag = settings_handler.set_format_date(
        new_setting_arg.value(), (sep) ? sep.value() : "-");
    const std::string flag_msg =
        (set_flag) ? std::format("Set \"date format\" to \"{}\"",
                                 new_setting_arg.value())
                   : "Invalid date format";
    return {.flag = flag_msg};
  } else if (it->second == "sort by") {
    settings_handler.set_sorterer(new_setting_arg.value());
    return {.flag = std::format("Set \"sorter\" to \"{}\"",
                                new_setting_arg.value())};
  } else {
    settings_handler.set_value_by_field(it->second, new_setting_arg.value());
    return {.flag = std::format("Set \"{}\" to \"{}\"", it->second,
                                new_setting_arg.value())};
  }

  return {.flag = Flag_Messages::bad_return("Interpreter::set_command")};
}

Parsing_Data Interpreter::mod_command(const std::vector<Token> &tokens) {
  if (tokens.size() < 4) {
    return {.flag = Flag_Messages::no_args()};
  }
  const auto option_arg = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::OPTION);
  if (!option_arg) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = "No '-' parameter provided"};
  }

  if (option_arg.value() == "name") {
    {
      const auto tasks_names =
          get_token_contents_if_contains_type(tokens, TokenType::TEXT);
      if (tasks_names.size() >= 2) {
        const std::string old_task_name = tasks_names[0];
        const std::string new_task_name = tasks_names[1];
        const auto old_task_ptr =
            entry_handler.get_entry_by_content<EntryTask>(old_task_name);
        if (!old_task_ptr) {
          return {.flag = Flag_Messages::invalid_args(),
                  .out_buffer =
                      std::format("No task named: \"{}\"", old_task_name)};
        }
        const auto new_task_ptr =
            EntryTask::Builder(*old_task_ptr).add_content(new_task_name).get();
        entry_handler.replace_entry(*old_task_ptr, *new_task_ptr);
        return {
            .flag = std::format("Modified task: \"{}\"", old_task_name),
            .out_buffer = std::format("Changed task name from \"{}\" to \"{}\"",
                                      old_task_name, new_task_name)};
      }
    }
    {
      const auto category_names =
          get_token_contents_if_contains_type(tokens, TokenType::CATEGORY_NAME);
      if (category_names.size() >= 2) {
        const std::string old_category_name = category_names[0];
        const std::string new_category_name = category_names[1];
        const auto old_category_ptr =
            entry_handler.get_entry_by_content<EntryCategory>(
                old_category_name);
        if (!old_category_ptr) {
          return {.flag = Flag_Messages::invalid_args(),
                  .out_buffer = std::format("No category named: @\"{}\"",
                                            old_category_name)};
        }
        EntryCategory new_category = EntryCategory(std::format(
            "{} \"{}\"", old_category_ptr->get_id(), new_category_name));
        entry_handler.replace_entry(*old_category_ptr, new_category);
        return {.flag = std::format("Modified category: @\"{}\"",
                                    old_category_name),
                .out_buffer =
                    std::format("Changed category name from @\"{}\" to @\"{}\"",
                                old_category_name, new_category_name)};
      }
    }
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer =
                "Invalid agruments provided. Propper syntax: \n\tscd mod "
                "task_name -name new_task_name\nor\n\tscd mod @category_name "
                "-name @new_category_name"};
  }

  return {.flag = Flag_Messages::bad_return("Interpreter::mod_command")};
}

constexpr std::vector<std::string>
Interpreter::get_token_contents_if_contains_type(
    const std::vector<Token> &tokens, const TokenType &token_type) {
  std::vector<std::string> result;
  auto copy_to_result = [&token_type, &result](const Token &token) {
    if (token.type == token_type) {
      result.emplace_back(token.content);
    }
  };
  std::for_each(tokens.begin(), tokens.end(), copy_to_result);
  return result;
}
