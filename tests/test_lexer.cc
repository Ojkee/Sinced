#include <cstdint>
#include <string>
#include <vector>

#include "../extern/include/catch.hpp"
#include "../include/syntax_analysis/lexer.hpp"

namespace Catch {
template <>
struct StringMaker<Token> {
  static std::string convert(Token const &value) {
    return "{" + std::to_string(static_cast<uint16_t>(value.type)) + ": " +
           value.content + "}\n";
  }
};
}  // namespace Catch

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
  const std::string user_input1 = "scd add \"task title\"";
  const auto result1 = Lexer::tokenize(user_input1, DDMMYYYY());
  const std::vector<Token> target1{Token{TokenType::TEXT, "scd"},
                                   Token{TokenType::COMMAND, "add"},
                                   Token{TokenType::TEXT, "task title"}};
  REQUIRE(result1 == target1);

  const std::string user_input2 = "scd add @\"Wholesome Category\"";
  const auto result2 = Lexer::tokenize(user_input2, DDMMYYYY());
  const std::vector<Token> target2{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::CATEGORY_NAME, "Wholesome Category"}};
  REQUIRE(result2 == target2);

  const std::string user_input3 =
      "scd add \"Name with more spaces\" @\"Wholesome Category name with more "
      "spaces\"";
  const auto result3 = Lexer::tokenize(user_input3, DDMMYYYY());
  const std::vector<Token> target3{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::TEXT, "Name with more spaces"},
      Token{TokenType::CATEGORY_NAME,
            "Wholesome Category name with more spaces"}};
  REQUIRE(result3 == target3);

  const std::string user_input4 =
      "scd add \"Name with @ spaces\" @\"Wholesome Category name with more "
      "spaces\"";
  const auto result4 = Lexer::tokenize(user_input4, DDMMYYYY());
  const std::vector<Token> target4{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::TEXT, "Name with @ spaces"},
      Token{TokenType::CATEGORY_NAME,
            "Wholesome Category name with more spaces"}};
  REQUIRE(result4 == target4);

  const std::string user_input5 = "scd add Task @Category 15-03-2030 -rw";
  const auto result5 = Lexer::tokenize(user_input5, DDMMYYYY());
  const std::vector<Token> target5{Token{TokenType::TEXT, "scd"},
                                   Token{TokenType::COMMAND, "add"},
                                   Token{TokenType::TEXT, "Task"},
                                   Token{TokenType::CATEGORY_NAME, "Category"},
                                   Token{TokenType::DATE, "15-03-2030"},
                                   Token{TokenType::OPTION, "rw"}};
  REQUIRE(result5 == target5);

  const std::string user_input6 = "scd rm @Category";
  const auto result6 = Lexer::tokenize(user_input6, DDMMYYYY());
  const std::vector<Token> target6{Token{TokenType::TEXT, "scd"},
                                   Token{TokenType::COMMAND, "rm"},
                                   Token{TokenType::CATEGORY_NAME, "Category"}};
  REQUIRE(result6 == target6);

  const std::string user_input7 = "scd add \"Michal 15:30-17:30\" @Korki -w";
  const auto result7 = Lexer::tokenize(user_input7, DDMMYYYY());
  const std::vector<Token> target7{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::TEXT, "Michal 15:30-17:30"},
      Token{TokenType::CATEGORY_NAME, "Korki"}, Token{TokenType::OPTION, "w"}};
  REQUIRE(result7 == target7);

  const std::string user_input8 = "scd         a  ";
  const auto result8 = Lexer::tokenize(user_input8, DDMMYYYY());
  const std::vector<Token> target8{Token{TokenType::TEXT, "scd"},
                                   Token{TokenType::TEXT, "a"}};
  REQUIRE(result8 == target8);

  const std::string user_input9 = "scd set YYYYMMDD -i";
  const auto result9 = Lexer::tokenize(user_input9, DDMMYYYY());
  const std::vector<Token> target9{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "set"},
      Token{TokenType::TEXT, "YYYYMMDD"}, Token{TokenType::OPTION, "i"}};
  REQUIRE(result9 == target9);

  const std::string user_input10 = "scd add \"My Awesome Task\"";
  const auto result10 = Lexer::tokenize(user_input10, DDMMYYYY());
  const std::vector<Token> target10{Token{TokenType::TEXT, "scd"},
                                    Token{TokenType::COMMAND, "add"},
                                    Token{TokenType::TEXT, "My Awesome Task"}};
  REQUIRE(result10 == target10);

  const std::string user_input11 = "add Task @Category 31-01-2003 -rd 6";
  const auto result11 = Lexer::tokenize(user_input11, DDMMYYYY());
  const std::vector<Token> target11{Token{TokenType::COMMAND, "add"},
                                    Token{TokenType::TEXT, "Task"},
                                    Token{TokenType::CATEGORY_NAME, "Category"},
                                    Token{TokenType::DATE, "31-01-2003"},
                                    Token{TokenType::OPTION, "rd"},
                                    Token{TokenType::NUMBER, "6"}};
  REQUIRE(result11 == target11);

  const std::string user_input12 = "add \"My taask\"";
  const auto result12 = Lexer::tokenize(user_input12, DDMMYYYY());
  const std::vector<Token> target12{Token{TokenType::COMMAND, "add"},
                                    Token{TokenType::TEXT, "My taask"}};
  REQUIRE(result12 == target12);

  const std::string user_input13 = "scd add @\"my category\"";
  const auto result13 = Lexer::tokenize(user_input13, DDMMYYYY());
  const std::vector<Token> target13{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "add"},
      Token{TokenType::CATEGORY_NAME, "my category"}};
  REQUIRE(result13 == target13);

  const std::string user_input14 =
      "scd mod @\"my category\" -name @\"my category1\"";
  const auto result14 = Lexer::tokenize(user_input14, DDMMYYYY());
  const std::vector<Token> target14{
      Token{TokenType::TEXT, "scd"}, Token{TokenType::COMMAND, "mod"},
      Token{TokenType::CATEGORY_NAME, "my category"},
      Token{TokenType::OPTION, "name"},
      Token{TokenType::CATEGORY_NAME, "my category1"}

  };
  REQUIRE(result14 == target14);
}

TEST_CASE("Test preprocessing input") {
  const std::vector<std::string> args1 = {"scd", "mod", "task", "@my category"};
  const std::string result1 = Lexer::preprocess(args1, 1);
  CHECK(result1 == "\"mod\" \"task\" @\"my category\" ");
}
