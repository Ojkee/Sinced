#ifndef INTERPTERE_HPP
#define INTERPTERE_HPP

#include <format>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../entry/entry_handler.hpp"
#include "../mcg_reader/reader.hpp"
#include "token.hpp"

class Flag_Messages {
 public:
  [[nodiscard]] constexpr static inline std::string invalid_args() {
    return "Invalid arguments";
  }
  [[nodiscard]] constexpr static inline std::string no_args() {
    return "No arguments provided";
  }
  [[nodiscard]] constexpr static inline std::string bad_return(
      const std::string &func_name) {
    return std::format("Bad return in: \"{}\"", func_name);
  }
  [[nodiscard]] constexpr static inline std::string no_task(
      const std::string &task_name) {
    return std::format("No task: \"{}\"", task_name);
  }
  [[nodiscard]] constexpr static inline std::string no_category(
      const std::string &category_name) {
    return std::format("No category: @\"{}\"", category_name);
  }
};

struct Parsing_Data {
  std::string flag{};
  std::string out_buffer{};
  bool session{false};
};

class Interpreter {
 public:
  Interpreter()
      : entry_handler("../../records/database/tasks.mdb",
                      "../../records/database/categories.mdb",
                      "../../records/database/relations.mdb"),
        tracker_handler("../../records/mcgs/tracker.mcg"),
        settings_handler("../../records/mcgs/settings.mcg") {}
  Interpreter(const std::string &tasks_path_,
              const std::string &categories_path_,
              const std::string &relations_path_,
              const std::string &tracker_path_,
              const std::string &settings_path_)
      : entry_handler(tasks_path_, categories_path_, relations_path_),
        tracker_handler(tracker_path_),
        settings_handler(settings_path_){};

  // returns message with parsing state
  [[nodiscard("Interpreter flag should not be ignored")]] Parsing_Data parse(
      const std::string &user_input);

 private:
  EntryHandler entry_handler;
  TrackerHandler tracker_handler;
  SettingsHandler settings_handler;

  [[nodiscard]] Parsing_Data add_command(const std::vector<Token> &tokens);
  [[nodiscard]] Parsing_Data log_command(const std::vector<Token> &tokens);
  [[nodiscard]] Parsing_Data set_command(const std::vector<Token> &tokens);

  // ADD
  template <typename... TokenTypes>
  [[nodiscard]] constexpr static bool contains_token_types(
      const std::vector<Token> &tokens, const TokenType &ttype,
      const TokenTypes &...ttypes) {
    return contains_token_type(tokens, ttype) &&
           contains_token_type(tokens, ttypes...);
  }
  [[nodiscard]] constexpr static bool contains_token_type(
      const std::vector<Token> &tokens, const TokenType &token_type);
  [[nodiscard]] constexpr static std::optional<std::string>
  get_token_content_if_contains_type(const std::vector<Token> &tokens,
                                     const TokenType &token_type);
  void add_new_relation(const std::string &task_id,
                        const std::string &category_id);
  [[nodiscard]] std::shared_ptr<EntryTask> build_task(
      const std::vector<Token> &tokens);

  // SET
  const std::unordered_map<std::string, std::string> set_params{
      {"df", "date format"}, {"ds", "date format separator"}, {"s", "sort by"}};
};

#endif  // INTERPTERE_HPP
