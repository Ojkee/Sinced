#ifndef HELP_MESSAGES_HPP_
#define HELP_MESSAGES_HPP_

#include <string>

class Help_Messages {
 public:
  [[nodiscard]] constexpr static inline std::string general() {
    return "- add - adds task or @category\n"
           "- log - logs task or @category\n"
           "- mod - modifies task name / -parameter / @category membership\n"
           "- rm - removes task/task from @category / @category / tasks with "
           "@category\n"
           "- set - modifies settings such as date format / separator in date "
           " / tasks filter\n"
           "- help - displays additional information about above commands\n";
  }
  [[nodiscard]] constexpr static inline std::string add() { return ""; }
  [[nodiscard]] constexpr static inline std::string add_syntax() {
    return "\tscd add <task / @category>\n\tscd add <task> <@category> "
           "<-parameter> [parameter flags]";
  }
  [[nodiscard]] constexpr static inline std::string log() { return ""; }
  [[nodiscard]] constexpr static inline std::string log_syntax() {
    return "\tscd add <task / @category>\n\tscd add <task> <-parameter> "
           "[parameter flags]";
  }
  [[nodiscard]] constexpr static inline std::string mod() { return ""; }
  [[nodiscard]] constexpr static inline std::string mod_syntax() {
    return "scd mod <task / @category> <-parameter> <parameter value>";
  }
  [[nodiscard]] constexpr static inline std::string rm() { return ""; }
  [[nodiscard]] constexpr static inline std::string rm_syntax() {
    return "scd rm <task / @category> [-a]";
  }
  [[nodiscard]] constexpr static inline std::string set() { return ""; }
  [[nodiscard]] constexpr static inline std::string set_syntax() {
    return "scd set <-parameter> <parameter flag>";
  }
  [[nodiscard]] constexpr static inline std::string help() { return ""; }
  [[nodiscard]] constexpr static inline std::string help_syntax() {
    return "scd help <command>";
  }
};

#endif  // !HELP_MESSAGES_HPP_
