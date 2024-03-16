#include "../extern/include/catch.hpp"

#include "../include/syntax_analysis/lexer.hpp"
#include <cstdint>
#include <string>

namespace Catch {
template <> struct StringMaker<Token> {
  static std::string convert(Token const &value) {
    return "{" + std::to_string(static_cast<uint16_t>(value.type)) + ": " +
           value.content + "}\n";
  }
};
} // namespace Catch

TEST_CASE("Utils - numeric") {
  REQUIRE(Lexer::is_unsigned_int("123123129") == true);
  REQUIRE(Lexer::is_unsigned_int("123123s") == false);
  REQUIRE(Lexer::is_unsigned_int("-12313") == false);
  REQUIRE(Lexer::is_unsigned_int("0") == true);
  REQUIRE(Lexer::is_unsigned_int("-0") == false);
}

TEST_CASE("Utils - strings") {
  REQUIRE(Lexer::is_command("add") == true);
  REQUIRE(Lexer::is_command("addd") == false);
  REQUIRE(Lexer::is_command("") == false);
  REQUIRE(Lexer::is_command("rem") == false);
}

TEST_CASE("Command tokenization") {
  const Lexer lexer = Lexer();

  const std::string user_input1 = "scd add \"task title\"";
  const auto result1 = lexer.tokenize(user_input1);
  const std::vector<Token> target1{Token{TokenType::TEXT, "scd"},
                                   Token{TokenType::COMMAND, "add"},
                                   Token{TokenType::TEXT, "task title"}};
  REQUIRE(result1 == target1);

  const std::string user_input2 = "scd add @\"Wholesome Category\"";
  const auto result2 = lexer.tokenize(user_input2);
  const std::vector<Token> target2{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::CATEGORY_NAME, "Wholesome Category"}};
  REQUIRE(result2 == target2);

  const std::string user_input3 =
      "scd add \"Name with more spaces\" @\"Wholesome Category name with more "
      "spaces\"";
  const auto result3 = lexer.tokenize(user_input3);
  const std::vector<Token> target3{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::TEXT, "Name with more spaces"},
      Token{TokenType::CATEGORY_NAME,
            "Wholesome Category name with more spaces"}};
  REQUIRE(result3 == target3);

  const std::string user_input4 =
      "scd add \"Name with @ spaces\" @\"Wholesome Category name with more "
      "spaces\"";
  const auto result4 = lexer.tokenize(user_input4);
  const std::vector<Token> target4{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::TEXT, "Name with @ spaces"},
      Token{TokenType::CATEGORY_NAME,
            "Wholesome Category name with more spaces"}};
  REQUIRE(result4 == target4);

  const std::string user_input5 = "scd add Task @Category 15-03-2030 -rw";
  const auto result5 = lexer.tokenize(user_input5);
  const std::vector<Token> target5{Token{TokenType::TEXT, "scd"},
                                   Token{TokenType::COMMAND, "add"},
                                   Token{TokenType::TEXT, "Task"},
                                   Token{TokenType::CATEGORY_NAME, "Category"},
                                   Token{TokenType::DATE, "15-03-2030"},
                                   Token{TokenType::OPTION, "rw"}};
  REQUIRE(result5 == target5);
}
