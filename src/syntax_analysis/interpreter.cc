#include "../../include/syntax_analysis/interpreter.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <iterator>
#include <memory>
#include <regex>
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
    // "add", "log", "mod", "rm", "set"
    const std::string command_str = tokens.begin()->content;
    if (command_str == "add") {
      return add_command(tokens);
    } else if (command_str == "log") {
      auto sorter = settings_handler.get_sorter();
      entry_handler.set_sorter(sorter);
      return log_command(tokens);
    } else if (command_str == "set") {
      return set_command(tokens);
    } else if (command_str == "mod") {
      return mod_command(tokens);
    } else if (command_str == "rm") {
      return rm_command(tokens);
    } else {
      return {.flag = "Command not implemented",
              .out_buffer = "Command not implemented"};
    }
  }

  return {.flag = Flag_Messages::bad_return("Interpreter::parse")};
}

Parsing_Data Interpreter::add_command(const std::vector<Token> &tokens) {
  if (tokens.size() <= 1) {
    return {.flag = Flag_Messages::no_args()};
  }

  auto task_arg =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::TEXT);
  auto category_arg = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::CATEGORY_NAME);

  if (!task_arg && !category_arg) {
    return {.flag = Flag_Messages::invalid_args()};
  } else if (task_arg && category_arg) {
    return add_task_to_category(tokens, task_arg.value(), category_arg.value());
  } else if (!task_arg && category_arg) {
    return add_new_category(category_arg.value());
  } else if (task_arg && !category_arg) {
    return add_new_task(tokens, task_arg.value());
  }
  return {.flag =
              Flag_Messages::bad_return("Interpreter::add_new_task_builder")};
}

Parsing_Data Interpreter::add_task_to_category(
    const std::vector<Token> &tokens, const std::string &task_name,
    const std::string &category_name) {
  auto category_ptr =
      entry_handler.get_entry_by_content<EntryCategory>(category_name);
  if (!category_ptr) {
    return {.flag = Flag_Messages::invalid_args()};
  }
  const auto task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(task_name);
  if (task_ptr) {
    const auto relation_ptr =
        entry_handler.get_task_relation_by_id(task_ptr->get_id());
    if (relation_ptr) {
      const auto category_relation_ptr =
          entry_handler.get_entry_by_id(relation_ptr->get_content_category());
      return {.flag = std::format("\"{}\" already in @\"{}\"", task_name,
                                  category_relation_ptr->get_content())};
    }
    add_new_relation(task_ptr->get_id(), category_ptr->get_id());
    return {.flag = std::format("Added: \"{}\" to @\"{}\"", task_name,
                                category_name)};
  }
  std::shared_ptr<EntryTask> new_task = build_task(tokens);
  entry_handler.add_entry_to_db(*new_task);
  add_new_relation(new_task->get_id(), category_ptr->get_id());
  return {.flag = std::format("Added new task: \"{}\" to @\"{}\"", task_name,
                              category_name)};
}

Parsing_Data Interpreter::add_new_category(const std::string &category_name) {
  auto category_ptr =
      entry_handler.get_entry_by_content<EntryCategory>(category_name);
  if (category_ptr) {
    return {.flag =
                std::format("Category: @\"{}\" already exists", category_name)};
  }
  const std::string category_token = std::format(
      "{} \"{}\"", tracker_handler.next_id("last category id"), category_name);
  const auto category = EntryCategory(category_token);
  entry_handler.add_entry_to_db(category);
  tracker_handler.increment_field_value("last category id");
  return {.flag = std::format("Added new category: @\"{}\"", category_name)};
}

Parsing_Data Interpreter::add_new_task(const std::vector<Token> &tokens,
                                       const std::string &task_name) {
  const auto task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(task_name);
  if (task_ptr) {
    return {
        .flag = Flag_Messages::invalid_args(),
        .out_buffer = std::format("Task: \"{}\" already exists", task_name)};
  }
  std::shared_ptr<EntryTask> new_task = build_task(tokens);
  entry_handler.add_entry_to_db(*new_task);
  return {.flag = std::format("Added new task: \"{}\"", task_name)};
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

void Interpreter::add_new_relation(const std::string &task_id,
                                   const std::string &category_id) {
  const std::string relation_token =
      std::format("{} {} {}", tracker_handler.next_id("last relation id"),
                  task_id, category_id);
  const EntryRelation relation = EntryRelation(relation_token);
  entry_handler.add_entry_to_db(relation);
  tracker_handler.increment_field_value("last relation id");
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
        .flag = Flag_Messages::invalid_args(),
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
    return {.flag = Flag_Messages::invalid_args(),
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
        return modify_task_name(tasks_names);
      }
    }
    {
      const auto category_names =
          get_token_contents_if_contains_type(tokens, TokenType::CATEGORY_NAME);
      if (category_names.size() >= 2) {
        return modify_category_name(category_names);
      }
    }
    return {
        .flag = Flag_Messages::invalid_args(),
        .out_buffer =
            "Invalid arguments provided. Propper syntax: \n\tscd mod "
            "<task_name> -name <new_task_name>\nor\n\tscd mod @<category_name> "
            "-name @<new_category_name>"};
  } else if (option_arg.value() == "status") {
    const auto task_status_texts =
        get_token_contents_if_contains_type(tokens, TokenType::TEXT);
    if (task_status_texts.size() >= 2) {
      return modify_task_status(task_status_texts);
    }
  } else if (option_arg.value() == "deadline") {
    const auto text_arg = Interpreter::get_token_content_if_contains_type(
        tokens, TokenType::TEXT);
    const auto deadline_arg = Interpreter::get_token_content_if_contains_type(
        tokens, TokenType::DATE);
    if (!(deadline_arg && text_arg)) {
      return {.flag = Flag_Messages::invalid_args(),
              .out_buffer =
                  "Invalid arguments provided. Propper syntax: \n\tscd mod "
                  "<task_name> -deadline <deadline in format>"};
    }
    return modify_task_deadline(text_arg.value(), deadline_arg.value());
  } else if (option_arg.value() == "r") {
    const auto task_recursive_args =
        get_token_contents_if_contains_type(tokens, TokenType::TEXT);
    if (task_recursive_args.size() < 2) {
      return {.flag = Flag_Messages::invalid_args(),
              .out_buffer =
                  "Invalid arguments provided. Propper syntax: \n\tscd mod "
                  "<task_name> -r <d/w/m/y number>"};
    }
    return modify_task_recursive(task_recursive_args);
  } else if (option_arg.value() == "to") {
    const auto text_arg = Interpreter::get_token_content_if_contains_type(
        tokens, TokenType::TEXT);
    const auto category_arg = Interpreter::get_token_content_if_contains_type(
        tokens, TokenType::CATEGORY_NAME);
    if (!text_arg || !category_arg) {
      return {.flag = Flag_Messages::no_args(),
              .out_buffer =
                  "Invalid arguments provided. Propper syntax: \n\tscd mod "
                  "<task_name> -to <@category_name>"};
    }
    return modify_task_relation(text_arg.value(), category_arg.value());
  }

  return {.flag = Flag_Messages::bad_return("Interpreter::mod_command")};
}

Parsing_Data Interpreter::modify_task_name(
    const std::vector<std::string> &tasks_names) {
  const std::string old_task_name = tasks_names[0];
  const std::string new_task_name = tasks_names[1];
  const auto old_task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(old_task_name);
  if (!old_task_ptr) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = std::format("No task named: \"{}\"", old_task_name)};
  }
  const auto new_task_ptr =
      EntryTask::Builder(*old_task_ptr).add_content(new_task_name).get();
  entry_handler.replace_entry(*old_task_ptr, *new_task_ptr);
  return {.flag = std::format("Modified task: \"{}\"", old_task_name),
          .out_buffer = std::format("Changed task name from \"{}\" to \"{}\"",
                                    old_task_name, new_task_name)};
}

Parsing_Data Interpreter::modify_category_name(
    const std::vector<std::string> &category_names) {
  const std::string old_category_name = category_names[0];
  const std::string new_category_name = category_names[1];
  const auto old_category_ptr =
      entry_handler.get_entry_by_content<EntryCategory>(old_category_name);
  if (!old_category_ptr) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer =
                std::format("No category named: @\"{}\"", old_category_name)};
  }
  EntryCategory new_category = EntryCategory(
      std::format("{} \"{}\"", old_category_ptr->get_id(), new_category_name));
  entry_handler.replace_entry(*old_category_ptr, new_category);
  return {
      .flag = std::format("Modified category: @\"{}\"", old_category_name),
      .out_buffer = std::format("Changed category name from @\"{}\" to @\"{}\"",
                                old_category_name, new_category_name)};
}

Parsing_Data Interpreter::modify_task_status(
    const std::vector<std::string> &task_status_texts) {
  const std::string task_name = task_status_texts[0];
  std::string status_str = task_status_texts[1];
  std::transform(status_str.begin(), status_str.end(), status_str.begin(),
                 ::tolower);
  Status _status;
  if (status_str == "ongoing") {
    _status = Status::ongoing;
  } else if (status_str == "done") {
    _status = Status::done;
  } else if (status_str == "canceled") {
    _status = Status::canceled;
  } else {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = "Invalid status provided"};
  }
  const auto task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(task_name);
  const auto new_task_ptr =
      EntryTask::Builder(*task_ptr).add_status(_status).get();
  entry_handler.replace_entry(*task_ptr, *new_task_ptr);
  return {.flag = std::format("Modified task: \"{}\"", task_name),
          .out_buffer = std::format("Status for task: \"{}\" has been changed",
                                    task_name)};
}

Parsing_Data Interpreter::modify_task_deadline(
    const std::string &task_name, const std::string &deadline_str) {
  const auto old_task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(task_name);
  if (!old_task_ptr) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = std::format("No task named: \"{}\"", task_name)};
  }
  BaseDate new_deadline = BaseDate();
  new_deadline.initialize_from_str(deadline_str);
  if (new_deadline < BaseDate::lower_bound_date()) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = "Invalid deadline"};
  }
  const auto new_task_ptr =
      EntryTask::Builder(*old_task_ptr).add_deadline(new_deadline).get();
  entry_handler.replace_entry(*old_task_ptr, *new_task_ptr);
  return {.flag = std::format("Modified task: \"{}\"", task_name),
          .out_buffer = std::format(
              "Deadline for task: \"{}\" has been changed", task_name)};
}

Parsing_Data Interpreter::modify_task_recursive(
    const std::vector<std::string> &task_recursive_args) {
  const std::string task_name = task_recursive_args[0];
  std::string recursive_str = task_recursive_args[1];
  const auto old_task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(task_name);
  if (!old_task_ptr) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = std::format("No task named: \"{}\"", task_name)};
  } else if (!old_task_ptr->get_deadline()) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer =
                "Can't change recursive parameters for task without deadline"};
  }

  bool found = false;
  std::vector<uint16_t> cals(3, 0);
  std::regex numb_expr("([dwmy])(\\d+)");
  for (std::smatch m; std::regex_search(recursive_str, m, numb_expr);
       recursive_str = m.suffix()) {
    found = true;
    std::string letter = m[1].str();
    uint16_t number = static_cast<uint16_t>(std::stoi(m[2].str()));
    if (letter == "d") {
      cals[0] += number;
    } else if (letter == "w") {
      cals[0] += number * 7;
    } else if (letter == "m") {
      cals[1] += number;
    } else {
      cals[2] += number;
    }
  }
  if (!found) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer =
                "Invalid arguments provided. Propper syntax: \n\tscd mod "
                "<task_name> -r <d/w/m/y number>"};
  }
  const auto new_task_ptr = EntryTask::Builder(*old_task_ptr)
                                .add_set_recurcive(cals[0], cals[1], cals[2])
                                .get();
  entry_handler.replace_entry(*old_task_ptr, *new_task_ptr);
  return {.flag = std::format("Modified task: \"{}\"", task_name),
          .out_buffer = std::format(
              "Recursive deadline parameter for task: \"{}\" has been changed",
              task_name)};
}

Parsing_Data Interpreter::modify_task_relation(
    const std::string &task_name, const std::string &category_name) {
  const auto task_ptr =
      entry_handler.get_entry_by_content<EntryTask>(task_name);
  const auto category_ptr =
      entry_handler.get_entry_by_content<EntryCategory>(category_name);
  if (!task_ptr) {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer = std::format("No task named: \"{}\"", task_name)};
  } else if (!category_ptr) {
    return {
        .flag = Flag_Messages::invalid_args(),
        .out_buffer = std::format("No category named: @\"{}\"", category_name)};
  } else {
    const auto relation_ptr =
        entry_handler.get_task_relation_by_id(task_ptr->get_id());
    if (relation_ptr &&
        relation_ptr->get_content_category() == category_ptr->get_id()) {
      return {.flag = "Task already in category",
              .out_buffer = std::format("Task: \"{}\" already in @\"{}\"",
                                        task_name, category_name)};
    } else if (relation_ptr) {
      entry_handler.remove_entry(*relation_ptr);
    }
    add_new_relation(task_ptr->get_id(), category_ptr->get_id());
    return {.flag = "Modified task relation",
            .out_buffer = std::format("Added task: \"{}\" to @\"{}\"",
                                      task_name, category_name)};
  }
}

Parsing_Data Interpreter::rm_command(const std::vector<Token> &tokens) {
  if (tokens.size() < 2) {
    return {.flag = Flag_Messages::no_args()};
  }

  const auto text_arg =
      Interpreter::get_token_content_if_contains_type(tokens, TokenType::TEXT);
  const auto category_arg = Interpreter::get_token_content_if_contains_type(
      tokens, TokenType::CATEGORY_NAME);
  if (text_arg) {
    const auto task_ptr =
        entry_handler.get_entry_by_content<EntryTask>(text_arg.value());
    if (!task_ptr) {
      return {
          .flag = Flag_Messages::invalid_args(),
          .out_buffer = std::format("No task named: \"{}\"", text_arg.value())};
    }
    entry_handler.remove_entry(*task_ptr);
    const auto relation_ptr =
        entry_handler.get_task_relation_by_id(task_ptr->get_id());
    if (relation_ptr) {
      entry_handler.remove_entry(*relation_ptr);
    }
    return {
        .flag = "Removed task",
        .out_buffer = std::format("Removed task: \"{}\"", text_arg.value())};
  } else if (category_arg) {
    const auto category_ptr =
        entry_handler.get_entry_by_content<EntryCategory>(category_arg.value());
    if (!category_ptr) {
      return {.flag = Flag_Messages::invalid_args(),
              .out_buffer = std::format("No category named: @\"{}\"",
                                        category_arg.value())};
    }
    const auto option_arg = Interpreter::get_token_content_if_contains_type(
        tokens, TokenType::OPTION);
    entry_handler.remove_entry(*category_ptr);
    entry_handler.remove_relations_by_category_id(category_ptr->get_id());
    if (option_arg && option_arg.value() == "a") {
      const auto tasks_ptrs =
          entry_handler.tasks_by_category_id(category_ptr->get_id());
      return {.flag = std::format("{}", tasks_ptrs.size())};  /// DEBUG
      for (std::shared_ptr<EntryTask> task_ptr : tasks_ptrs) {
        entry_handler.remove_entry(*task_ptr);
      }
      return {.flag = "Removed category with tasks",
              .out_buffer = std::format("Removed @\"{}\" with all tasks",
                                        category_arg.value())};
    }
    return {.flag = "Removed category",
            .out_buffer =
                std::format("Removed category: @\"{}\"", category_arg.value())};
  } else {
    return {.flag = Flag_Messages::invalid_args(),
            .out_buffer =
                "Invalid arguments provided. Propper syntax: \n\tscd rm "
                "<task_name> \nor\n\tscd rm <@castegory_name> [-a optional: "
                "removes all tasks from tat @category]"};
  }

  return {.flag = Flag_Messages::bad_return("Interpreter::rm")};
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
