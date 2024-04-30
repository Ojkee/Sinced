#ifndef INTERPTERE_HPP
#define INTERPTERE_HPP

#include "../entry/entry_handler.hpp"
#include "../mcg_reader/reader.hpp"
#include "token.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

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
        tracker_handler(tracker_path_), settings_handler(settings_path_){};

  // returns message with parsing state
  [[nodiscard("Interpreter flag should not be ignored")]] std::string
  parse(const std::string &user_input);

private:
  EntryHandler entry_handler;
  TrackerHandler tracker_handler;
  SettingsHandler settings_handler;

  [[nodiscard]] constexpr static bool
  contains_token_type(const std::vector<Token> &tokens,
                      const TokenType &token_type);
  template <typename... TokenTypes>
  [[nodiscard]] constexpr static bool
  contains_token_types(const std::vector<Token> &tokens, const TokenType &ttype,
                       const TokenTypes &...ttypes) {
    return contains_token_type(tokens, ttype) &&
           contains_token_type(tokens, ttypes...);
  }
  [[nodiscard]] constexpr static std::optional<std::string>
  get_token_content_if_contains_type(const std::vector<Token> &tokens,
                                     const TokenType &token_type);
  void add_new_relation(const std::string &task_id,
                        const std::string &category_id);

  [[nodiscard]] std::shared_ptr<EntryTask>
  build_task(const std::vector<Token> &tokens);
  std::string add_command(const std::vector<Token> &tokens);
  std::string add_new_task(const std::vector<Token> &tokens);
  std::string add_new_category(const std::vector<Token> &tokens);
  std::string add_new_task_builder(const std::vector<Token> &tokens);
  std::string add_new_task_to_category(const std::vector<Token> &tokens);
};

#endif // INTERPTERE_HPP
