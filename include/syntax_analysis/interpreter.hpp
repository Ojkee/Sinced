#ifndef INTERPTERE_HPP
#define INTERPTERE_HPP

#include <linux/limits.h>
#include <unistd.h>

#include <cstdint>
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
};

struct Parsing_Data {
  std::string flag{};
  std::string out_buffer{};
  bool session{false};
};

class Interpreter {
 public:
  Interpreter()
      : entry_handler(get_records_path() + "/database/tasks.mdb",
                      get_records_path() + "/database/categories.mdb",
                      get_records_path() + "/database/relations.mdb"),
        tracker_handler(get_records_path() + "/mcgs/tracker.mcg"),
        settings_handler(get_records_path() + "/mcgs/settings.mcg") {}
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
  [[nodiscard]] Parsing_Data mod_command(const std::vector<Token> &tokens);
  [[nodiscard]] Parsing_Data rm_command(const std::vector<Token> &tokens);
  [[nodiscard]] Parsing_Data help_command(const std::vector<Token> &tokens);
  [[nodiscard]] Parsing_Data reset_command(const std::vector<Token> &tokens);

  // ADD
  [[nodiscard]] Parsing_Data add_task_to_category(
      const std::vector<Token> &tokens, const std::string &task_name,
      const std::string &category_name);
  [[nodiscard]] Parsing_Data add_new_category(const std::string &category_name);
  [[nodiscard]] Parsing_Data add_new_task(const std::vector<Token> &tokens,
                                          const std::string &task_name);
  [[nodiscard]] std::shared_ptr<EntryTask> build_task(
      const std::vector<Token> &tokens);
  void add_new_relation(const std::string &task_id,
                        const std::string &category_id);

  // SET
  const std::unordered_map<std::string, std::string> set_params{
      {"df", "date format"}, {"ds", "date format separator"}, {"s", "sort by"}};

  // LOG
  [[nodiscard]] Parsing_Data parse_log_filter(const std::string &option,
                                              const std::vector<Token> &tokens);

  // MOD
  [[nodiscard]] Parsing_Data modify_task_name(
      const std::vector<std::string> &tasks_names);
  [[nodiscard]] Parsing_Data modify_category_name(
      const std::vector<std::string> &category_names);
  [[nodiscard]] Parsing_Data modify_task_status(
      const std::vector<std::string> &task_status_texts);
  [[nodiscard]] Parsing_Data modify_task_deadline(
      const std::string &task_name, const std::string &deadline_str);
  [[nodiscard]] Parsing_Data modify_task_recursive(
      const std::vector<std::string> &task_recursive_args);
  [[nodiscard]] Parsing_Data modify_task_relation(
      const std::string &task_name, const std::string &category_name);

  // RM

  // UTILS
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
  [[nodiscard]] constexpr std::vector<std::string>
  get_token_contents_if_contains_type(const std::vector<Token> &tokens,
                                      const TokenType &token_type);

  std::string get_records_path() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::string result_str =
        (count > 0) ? std::string(result, static_cast<uint64_t>(count))
                    : std::string(result, 0);
    std::string directory_path =
        result_str.substr(0, result_str.find_last_of("/\\"));
    return directory_path + "/../../records";
  }
  void load_date_format_settings();
};

#endif  // INTERPTERE_HPP
