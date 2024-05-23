#ifndef LEXER_HPP
#define LEXER_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "../../include/date/format_date.hpp"
#include "token.hpp"

class Lexer {
 public:
  Lexer() = default;

  [[nodiscard]] constexpr static std::vector<Token> tokenize(
      const std::string &line, const FormatDate &date_format) {
    std::string word;
    std::vector<Token> tokens;
    bool in_quotes = false;
    auto append_and_clear = [&tokens, &date_format](std::string &word) {
      tokens.emplace_back(Lexer::get_token_type(word, date_format),
                          Lexer::process_content(word));
      word.clear();
    };
    for (const char &c : line) {
      if (c == '\"') {
        in_quotes = !in_quotes;
        if (!in_quotes && !word.empty()) {
          append_and_clear(word);
        }
      } else if (std::isspace(c) && !in_quotes) {
        if (!word.empty()) {
          append_and_clear(word);
        }
      } else [[likely]] {
        word += c;
      }
    }
    if (!word.empty()) {
      tokens.emplace_back(Token{Lexer::get_token_type(word, date_format),
                                Lexer::process_content(word)});
    }
    return tokens;
  }

  [[nodiscard]] constexpr static TokenType get_token_type(
      const std::string &input_string, const FormatDate &format_date) {
    if (input_string.empty()) {
      return TokenType::BLANK_SPACE;
    }
    if (input_string.front() == '@') {
      return TokenType::CATEGORY_NAME;
    } else if (input_string.front() == '-') {
      return TokenType::OPTION;
    } else if (Lexer::is_unsigned_int(input_string)) {
      return TokenType::NUMBER;
    } else if (Lexer::is_command(input_string)) {
      return TokenType::COMMAND;
    } else if (Lexer::is_date(input_string, format_date)) {
      return TokenType::DATE;
    }
    return TokenType::TEXT;
  }

  [[nodiscard]] constexpr static std::string process_content(
      const std::string &content) {
    if (content.front() == '@' || content.front() == '-') {
      return content.substr(1);
    }
    return content;
  }

  [[nodiscard]] static constexpr bool is_unsigned_int(
      const std::string &input_string) {
    return std::find_if(input_string.begin(), input_string.end(),
                        [](const auto &c) { return !std::isdigit(c); }) ==
           input_string.end();
  }

  [[nodiscard]] static constexpr bool is_command(
      const std::string &input_string) {
    return std::binary_search(commands_set.begin(), commands_set.end(),
                              input_string);
  }

  [[nodiscard]] constexpr static bool is_date(
      const std::string &input_string, const FormatDate &date_formatter) {
    return date_formatter.is_valid(input_string);
  }

  [[nodiscard]] static std::string preprocess(
      const std::vector<std::string> &args, const int64_t &start_concat) {
    if (start_concat >= static_cast<int64_t>(args.size())) {
      return "";
    }
    std::stringstream result;
    std::for_each(args.begin() + start_concat, args.end(),
                  [&result](const std::string &argv) {
                    if (argv.front() == '@' || argv.front() == '-') {
                      result << argv.front() << "\"" << process_content(argv)
                             << "\" ";
                    } else {
                      result << "\"" << argv << "\" ";
                    }
                  });
    return result.str();
  }

 private:
  constexpr static auto commands_set = {
      "add", "help", "log", "mod", "reset", "rm", "set"};  // KEEP LEXICOGRAPHIC
};

#endif  // LEXER_HPP
