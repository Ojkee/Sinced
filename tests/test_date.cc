#include <memory>
#include <string>

#include "../extern/include/catch.hpp"
#include "../include/date/base_date.hpp"
#include "../include/date/format_date.hpp"

namespace Catch {
template <>
struct StringMaker<BaseDate> {
  static std::string convert(BaseDate const &value) {
    return "{" + std::string(value) + "}\n";
  }
};
}  // namespace Catch

TEST_CASE("Is leap year") {
  CHECK(BaseDate::is_leap(2000) == true);
  CHECK(BaseDate::is_leap(2100) == false);
  CHECK(BaseDate::is_leap(2004) == true);
  CHECK(BaseDate::is_leap(2400) == true);
  CHECK(BaseDate::is_leap(2401) == false);
  CHECK(BaseDate::is_leap(2001) == false);
  CHECK(BaseDate::is_leap(1991) == false);
  CHECK(BaseDate::is_leap(1992) == true);
  CHECK(BaseDate::is_leap(2023) == false);
  CHECK(BaseDate::is_leap(2094) == false);
  CHECK(BaseDate::is_leap(2092) == true);
  CHECK(BaseDate::is_leap(2096) == true);
  CHECK(BaseDate::is_leap(2095) == false);
}

TEST_CASE("Validation") {
  BaseDate b1 = BaseDate(-4, 2, 2023);
  CHECK(std::string(b1) == "01-02-2023");

  BaseDate b2 = BaseDate(50, 2, 2024);
  CHECK(std::string(b2) == "29-02-2024");

  BaseDate b3 = BaseDate(30, 2, -99);
  CHECK(std::string(b3) == "28-02-1970");

  BaseDate b4 = BaseDate(40, 15, -99);
  CHECK(std::string(b4) == "31-12-1970");

  BaseDate b5 = BaseDate(40, 0, -99);
  CHECK(std::string(b5) == "31-01-1970");
}

TEST_CASE("Converting date -> days") {
  BaseDate b1 = BaseDate(23, 10, 2023);
  CHECK(BaseDate::date_to_days(b1) == 19653);

  BaseDate b2 = BaseDate(28, 3, 2063);
  CHECK(BaseDate::date_to_days(b2) == 34054);

  BaseDate b3 = BaseDate(29, 2, 1972);
  CHECK(BaseDate::date_to_days(b3) == 789);

  BaseDate b4 = BaseDate(1, 3, 1972);
  CHECK(BaseDate::date_to_days(b4) == 790);

  BaseDate b5 = BaseDate(29, 2, 2024);
  CHECK(BaseDate::date_to_days(b5) == 19782);

  BaseDate b6 = BaseDate(1, 3, 2024);
  CHECK(BaseDate::date_to_days(b6) == 19783);

  BaseDate b7 = BaseDate(29, 2, 2048);
  CHECK(BaseDate::date_to_days(b7) == 28548);

  BaseDate b8 = BaseDate(1, 3, 2048);
  CHECK(BaseDate::date_to_days(b8) == 28549);

  BaseDate b9 = BaseDate(9, 1, 2028);
  CHECK(BaseDate::date_to_days(b9) == 21192);

  BaseDate b10 = BaseDate(9, 12, 2028);
  CHECK(BaseDate::date_to_days(b10) == 21527);

  BaseDate b11 = BaseDate(16, 5, 2027);
  CHECK(BaseDate::date_to_days(b11) == 20954);

  BaseDate b12 = BaseDate(16, 1, 2028);
  CHECK(BaseDate::date_to_days(b12) == 21199);

  BaseDate b13 = BaseDate(5, 1, 1973);
  CHECK(BaseDate::date_to_days(b13) == 1100);

  BaseDate b14 = BaseDate(1, 3, 2025);
  CHECK(BaseDate::date_to_days(b14) == 20148);

  BaseDate b15 = BaseDate(4, 10, 2024);
  CHECK(BaseDate::date_to_days(b15) == 20000);

  BaseDate b16 = BaseDate(30, 7, 2065);
  CHECK(BaseDate::date_to_days(b16) == 34909);

  BaseDate b17 = BaseDate(30, 7, 2069);
  CHECK(BaseDate::date_to_days(b17) == 36370);

  BaseDate b18 = BaseDate(12, 12, 2069);
  CHECK(BaseDate::date_to_days(b18) == 36505);

  BaseDate b19 = BaseDate(31, 12, 2069);
  CHECK(BaseDate::date_to_days(b19) == 36524);

  BaseDate b20 = BaseDate(1, 1, 2070);
  CHECK(BaseDate::date_to_days(b20) == 36525);

  BaseDate b21 = BaseDate(31, 12, 2140);
  CHECK(BaseDate::date_to_days(b21) == 62456);

  BaseDate b22 = BaseDate(1, 1, 2141);
  CHECK(BaseDate::date_to_days(b22) == 62457);

  BaseDate b23 = BaseDate(31, 12, 2090);
  CHECK(BaseDate::date_to_days(b23) == 44194);

  BaseDate b24 = BaseDate(29, 07, 2094);
  CHECK(BaseDate::date_to_days(b24) == 45500);

  BaseDate b25 = BaseDate(31, 12, 2094);
  CHECK(BaseDate::date_to_days(b25) == 45655);

  BaseDate b26 = BaseDate(1, 1, 2095);
  CHECK(BaseDate::date_to_days(b26) == 45656);

  BaseDate b27 = BaseDate(1, 3, 2070);
  CHECK(BaseDate::date_to_days(b27) == 36584);
}

TEST_CASE("Converting days -> date") {
  BaseDate b1 = BaseDate::days_to_date(19653);
  CHECK(std::string(b1) == "23-10-2023");

  BaseDate b2 = BaseDate::days_to_date(34054);
  CHECK(std::string(b2) == "28-03-2063");

  BaseDate b3 = BaseDate::days_to_date(789);
  CHECK(std::string(b3) == "29-02-1972");

  BaseDate b4 = BaseDate::days_to_date(790);
  CHECK(std::string(b4) == "01-03-1972");

  BaseDate b5 = BaseDate::days_to_date(19782);
  CHECK(std::string(b5) == "29-02-2024");

  BaseDate b6 = BaseDate::days_to_date(19783);
  CHECK(std::string(b6) == "01-03-2024");

  BaseDate b7 = BaseDate::days_to_date(28548);
  CHECK(std::string(b7) == "29-02-2048");

  BaseDate b8 = BaseDate::days_to_date(28549);
  CHECK(std::string(b8) == "01-03-2048");

  BaseDate b9 = BaseDate::days_to_date(20954);
  CHECK(std::string(b9) == "16-05-2027");

  BaseDate b10 = BaseDate::days_to_date(21340);
  CHECK(std::string(b10) == "05-06-2028");

  BaseDate b11 = BaseDate::days_to_date(21199);
  CHECK(std::string(b11) == "16-01-2028");

  BaseDate b12 = BaseDate::days_to_date(20000);
  CHECK(std::string(b12) == "04-10-2024");

  BaseDate b13 = BaseDate::days_to_date(45500);
  CHECK(std::string(b13) == "29-07-2094");
}

TEST_CASE("Adding days to date") {
  BaseDate b0 = BaseDate(5, 12, 2001);
  b0.add_days(0);
  CHECK(std::string(b0) == "05-12-2001");

  BaseDate b1 = BaseDate(5, 12, 2001);
  b1.add_days(100);
  CHECK(std::string(b1) == "15-03-2002");

  BaseDate b2 = BaseDate(5, 12, 2019);
  b2.add_days(100);
  CHECK(std::string(b2) == "14-03-2020");

  BaseDate b3 = BaseDate(16, 4, 2010);
  b3.add_days(365);
  CHECK(std::string(b3) == "16-04-2011");

  BaseDate b4 = BaseDate(16, 4, 2007);
  b4.add_days(365);
  CHECK(std::string(b4) == "15-04-2008");

  BaseDate b5 = BaseDate(15, 3, 2002);
  b5.add_days(-100);
  CHECK(std::string(b5) == "05-12-2001");

  BaseDate b6 = BaseDate(14, 3, 2020);
  b6.add_days(-100);
  CHECK(std::string(b6) == "05-12-2019");

  BaseDate b7 = BaseDate(16, 4, 2011);
  b7.add_days(-365);
  CHECK(std::string(b7) == "16-04-2010");

  BaseDate b8 = BaseDate(15, 4, 2008);
  b8.add_days(-365);
  CHECK(std::string(b8) == "16-04-2007");

  BaseDate b9 = BaseDate(2, 1, 1970);
  b9.add_days(-365);
  CHECK(std::string(b9) == "01-01-1970");

  BaseDate b10 = BaseDate(10, 1, 2001);
  b10.add_days(2000);
  CHECK(std::string(b10) == "03-07-2006");

  BaseDate b11 = BaseDate(3, 7, 2006);
  b11.add_days(-2000);
  CHECK(std::string(b11) == "10-01-2001");

  BaseDate b12 = BaseDate(29, 2, 2024);
  b12.add_days(1);
  CHECK(std::string(b12) == "01-03-2024");

  BaseDate b13 = BaseDate(16, 2, 2024);
  b13.add_days(14);
  CHECK(std::string(b13) == "01-03-2024");

  BaseDate b14 = BaseDate(1, 4, 2024);
  b14.add_days(-14);
  CHECK(std::string(b14) == "18-03-2024");

  BaseDate b15 = BaseDate(8, 2, 2024);
  b15.add_days(-14);
  CHECK(std::string(b15) == "25-01-2024");

  BaseDate b16 = BaseDate(1, 3, 2024);
  b16.add_days(-14);
  CHECK(std::string(b16) == "16-02-2024");

  BaseDate b17 = BaseDate(1, 3, 2024);
  b17.add_days(-1);
  CHECK(std::string(b17) == "29-02-2024");
}

TEST_CASE("Adding months to date") {
  BaseDate b1 = BaseDate(5, 5, 2024);
  b1.add_months(1);
  CHECK(std::string(b1) == "05-06-2024");

  BaseDate b2 = BaseDate(31, 1, 2024);
  b2.add_months(1);
  CHECK(std::string(b2) == "29-02-2024");

  BaseDate b3 = BaseDate(31, 1, 2024);
  b3.add_months(12);
  CHECK(std::string(b3) == "31-01-2025");

  BaseDate b4 = BaseDate(31, 1, 2024);
  b4.add_months(13);
  CHECK(std::string(b4) == "28-02-2025");

  BaseDate b5 = BaseDate(1, 2, 2001);
  b5.add_months(22);
  CHECK(std::string(b5) == "01-12-2002");

  BaseDate b6 = BaseDate(1, 2, 2001);
  b6.add_months(100);
  CHECK(std::string(b6) == "01-06-2009");

  BaseDate b7 = BaseDate(31, 1, 2024);
  b7.add_months(-12);
  CHECK(std::string(b7) == "31-01-2023");

  BaseDate b8 = BaseDate(29, 2, 2024);
  b8.add_months(-13);
  CHECK(std::string(b8) == "29-01-2023");

  BaseDate b9 = BaseDate(1, 12, 2002);
  b9.add_months(-22);
  CHECK(std::string(b9) == "01-02-2001");

  BaseDate b10 = BaseDate(5, 5, 2024);
  b10.add_months(-1);
  CHECK(std::string(b10) == "05-04-2024");

  BaseDate b11 = BaseDate(31, 3, 2024);
  b11.add_months(-1);
  CHECK(std::string(b11) == "29-02-2024");

  BaseDate b12 = BaseDate(1, 6, 2009);
  b12.add_months(-100);
  CHECK(std::string(b12) == "01-02-2001");
}

TEST_CASE("Adding years to date") {
  BaseDate b1 = BaseDate(29, 2, 2000);
  b1.add_years(1);
  CHECK(std::string(b1) == "28-02-2001");

  BaseDate b2 = BaseDate(29, 2, 2000);
  b2.add_years(-10000);
  CHECK(std::string(b2) == "01-01-1970");

  BaseDate b3 = BaseDate(29, 2, 2000);
  b3.add_years(10000);
  CHECK(std::string(b3) == "31-12-9999");

  BaseDate b4 = BaseDate(29, 2, 2000);
  b4.add_years(100);
  CHECK(std::string(b4) == "28-02-2100");

  BaseDate b5 = BaseDate(29, 2, 2000);
  b5.add_years(110);
  CHECK(std::string(b5) == "28-02-2110");
}

TEST_CASE("Displaying different formats of date") {
  BaseDate b1 = BaseDate(1, 4, 2001);
  CHECK(std::string(b1) == "01-04-2001");

  BaseDate b2 = BaseDate(29, 5, 2002);
  b2.set_formatter(std::make_unique<MMDDYYYY>());
  CHECK(std::string(b2) == "05-29-2002");

  BaseDate b3 = BaseDate(9, 5, 2002);
  b3.set_formatter(std::make_unique<DDMMYY>());
  CHECK(std::string(b3) == "09-05-02");

  BaseDate b4 = BaseDate(9, 5, 2002);
  b4.set_formatter(std::make_unique<MMDDYY>());
  CHECK(std::string(b4) == "05-09-02");

  BaseDate b5 = BaseDate(9, 5, 2002);
  b5.set_formatter(std::make_unique<MMDDYY>("/"));
  CHECK(std::string(b5) == "05/09/02");

  BaseDate b6 = BaseDate(9, 5, 2002);
  b6.set_formatter(std::make_unique<YYYYMMDD>("#"));
  CHECK(std::string(b6) == "2002#05#09");

  BaseDate b7 = BaseDate(9, 5, 2002);
  b7.set_formatter(std::make_unique<DMY>(""));
  CHECK(std::string(b7) == "9502");

  BaseDate b8 = BaseDate(9, 5, 2002);
  b8.set_formatter(std::make_unique<MDY>(""));
  CHECK(std::string(b8) == "5902");

  BaseDate b9 = BaseDate(9, 5, 2002);
  b9.set_formatter(std::make_unique<MonthDY>("dsad"));
  CHECK(std::string(b9) == "May 9, 2002");

  BaseDate b10 = BaseDate(9, 5, 2002);
  b10.set_formatter(std::make_unique<Roman>());
  CHECK(std::string(b10) == "9 V 2002");

  BaseDate b11 = BaseDate(99, 2, 1800);
  b11.set_formatter(std::make_unique<DDMMYY>("."));
  CHECK(std::string(b11) == "28.02.70");
}

TEST_CASE("Date formatter validation checker") {
  DDMMYY f1 = DDMMYY(",");
  std::string d1 = "12,03,23";
  CHECK(f1.is_valid(d1) == true);

  DDMMYY f2 = DDMMYY(",");
  std::string d2 = "22,03-23";
  CHECK(f2.is_valid(d2) == false);

  DDMMYY f3 = DDMMYY();
  std::string d3 = "33-03-2033";
  CHECK(f3.is_valid(d3) == false);

  DDMMYY f4 = DDMMYY();
  std::string d4 = "1-1-22";
  CHECK(f4.is_valid(d4) == true);

  DDMMYY f5 = DDMMYY("/");
  std::string d5 = "1/1/22";
  CHECK(f5.is_valid(d5) == true);

  YYYYMMDD f6 = YYYYMMDD();
  std::string d6 = "2001-1-2";
  CHECK(f6.is_valid(d6) == true);

  YYYYMMDD f7 = YYYYMMDD("?");
  std::string d7 = "2001?01?2";
  CHECK(f7.is_valid(d7) == true);

  YYYYMMDD f8 = YYYYMMDD("test");
  std::string d8 = "2001test01test2";
  CHECK(f8.is_valid(d8) == true);

  YYYYMMDD f9 = YYYYMMDD("?..d");
  std::string d9 = "2001?..d01?..d2";
  CHECK(f9.is_valid(d9) == true);

  YYYYMMDD f10 = YYYYMMDD("??");
  std::string d10 = "2001..1..2";
  CHECK(f10.is_valid(d10) == false);

  YYYYMMDD f11 = YYYYMMDD("??");
  std::string d11 = "2001??1??2";
  CHECK(f11.is_valid(d11) == true);

  YYMMDD f12 = YYMMDD("??");
  std::string d12 = "01??1??2";
  CHECK(f12.is_valid(d12) == true);

  YYMMDD f13 = YYMMDD("??");
  std::string d13 = "1??1??2";
  CHECK(f13.is_valid(d13) == false);

  YYMMDD f14 = YYMMDD("..");
  std::string d14 = "01..1..2";
  CHECK(f14.is_valid(d14) == true);

  YYMMDD f15 = YYMMDD("..");
  std::string d15 = "01?.1?.2";
  CHECK(f15.is_valid(d15) == false);

  YYMMDD f16 = YYMMDD("*");
  std::string d16 = "01.1.2";
  CHECK(f16.is_valid(d16) == false);

  YYMMDD f17 = YYMMDD("*");
  std::string d17 = "01*1*2";
  CHECK(f17.is_valid(d17) == true);
}

TEST_CASE("Day of week from date") {
  BaseDate b1 = BaseDate(1, 7, 1970);
  CHECK(b1.day_of_week() == 3);

  BaseDate b2 = BaseDate(1, 7, 1902);
  CHECK(b2.day_of_week_name() == "Wednesday");

  BaseDate b3 = BaseDate(16, 4, 2010);
  CHECK(b3.day_of_week() == 5);

  BaseDate b4 = BaseDate(16, 4, 2010);
  CHECK(b4.day_of_week_name() == "Friday");

  BaseDate b5 = BaseDate(14, 9, 3989);
  CHECK(b5.day_of_week() == 4);

  BaseDate b6 = BaseDate(14, 9, 3989);
  CHECK(b6.day_of_week_name() == "Thursday");

  BaseDate b7 = BaseDate(15, 8, 2050);
  CHECK(b7.day_of_week() == 1);

  BaseDate b8 = BaseDate(15, 8, 2050);
  CHECK(b8.day_of_week_name() == "Monday");

  BaseDate b9 = BaseDate(3, 11, 2001);
  CHECK(b9.day_of_week() == 6);

  BaseDate b10 = BaseDate(3, 11, 2001);
  CHECK(b10.day_of_week_name() == "Saturday");
}

// TEST_CASE("Current time related")
// {
//   BaseDate b1 = BaseDate::today();
//   CHECK(std::string(b1) == "01-02-2024");

//   BaseDate b2 = BaseDate(1, 2, 2024);
//   CHECK(b2.remaining_days() == 0);

//   BaseDate b3 = BaseDate(31, 1, 2024);
//   CHECK(b3.remaining_days() == -1);

//   BaseDate b4 = BaseDate(15, 2, 2024);
//   CHECK(b4.remaining_days() == 14);
// }

TEST_CASE("Lower bound date") {
  BaseDate b1 = BaseDate::lower_bound_date();
  CHECK(std::string(b1) == "01-01-1970");
}

TEST_CASE("comperison operators overloading") {
  BaseDate b1_1 = BaseDate(18, 11, 2000);
  BaseDate b1_2 = BaseDate(3, 11, 2001);
  const bool r1 = b1_1 < b1_2;
  CHECK(r1 == true);

  BaseDate b2_1 = BaseDate(29, 2, 2000);
  BaseDate b2_2 = BaseDate(30, 2, 2000);
  const bool r2 = b2_1 < b2_2;
  CHECK(r2 == false);

  BaseDate b3_1 = BaseDate(23, 2, 2000);
  BaseDate b3_2 = BaseDate(23, 2, 2000);
  const bool r3 = b3_1 == b3_2;
  CHECK(r3 == true);

  BaseDate b4_1 = BaseDate(24, 2, 2000);
  BaseDate b4_2 = BaseDate(21, 2, 2000);
  const bool r4 = b4_1 == b4_2;
  CHECK(r4 == false);

  BaseDate b5_1 = BaseDate(25, 2, 2000);
  BaseDate b5_2 = BaseDate(21, 2, 2000);
  const bool r5 = b5_1 <= b5_2;
  CHECK(r5 == false);

  BaseDate b6_1 = BaseDate(25, 2, 1999);
  BaseDate b6_2 = BaseDate(21, 2, 2000);
  const bool r6 = b6_1 <= b6_2;
  CHECK(r6 == true);

  BaseDate b7_1 = BaseDate(25, 2, 1999);
  BaseDate b7_2 = BaseDate(21, 2, 2000);
  BaseDate b7_3 = BaseDate(21, 2, 2000);
  const bool r7 = b7_1 <= b7_2 && b7_2 <= b7_3;
  CHECK(r7 == true);

  BaseDate b8_1 = BaseDate(25, 2, 1999);
  BaseDate b8_2 = BaseDate(21, 3, 2000);
  BaseDate b8_3 = BaseDate(21, 2, 2000);
  const bool r8 = b8_1 <= b8_2 && b8_2 <= b8_3;
  CHECK(r8 == false);

  BaseDate b9_1 = BaseDate(25, 2, 2001);
  BaseDate b9_2 = BaseDate(21, 2, 2000);
  BaseDate b9_3 = BaseDate(21, 2, 2005);
  const bool r9 = b9_1 <= b9_2 && b9_2 <= b9_3;
  CHECK(r9 == false);

  BaseDate b10_1 = BaseDate::today();
  BaseDate b10_2 = BaseDate(21, 2, 2000);
  const bool r10 = b10_1 <= b10_2;
  CHECK(r10 == false);

  BaseDate b11_1 = BaseDate::today();
  BaseDate b11_2 = BaseDate(26, 2, 2024);
  const bool r11 = b11_1 <= b11_2;
  CHECK(r11 == false);
}

TEST_CASE("Initialization from string") {
  BaseDate b1 = BaseDate();
  b1.set_formatter(std::make_unique<DDMMYYYY>());
  b1.initialize_from_str("20-04-2054");
  const std::string t1 = "20-04-2054";
  const std::string r1 = std::string(b1);
  CHECK(r1 == t1);

  BaseDate b2 = BaseDate();
  b2.set_formatter(std::make_unique<DDMMYYYY>("."));
  b2.initialize_from_str("00.04.2054");
  const std::string t2 = "01.04.2054";
  const std::string r2 = std::string(b2);
  CHECK(r2 == t2);

  BaseDate b3 = BaseDate();
  b3.set_formatter(std::make_unique<DDMMYYYY>("--"));
  b3.initialize_from_str("00--04--3054");
  const std::string t3 = "01--04--3054";
  const std::string r3 = std::string(b3);
  CHECK(r3 == t3);

  BaseDate b4 = BaseDate();
  b4.set_formatter(std::make_unique<DMY>("--"));
  b4.initialize_from_str("00--04--54");
  const BaseDate tb4 = BaseDate(1, 4, 2054);
  CHECK(b4 == tb4);

  BaseDate b5 = BaseDate();
  b5.set_formatter(std::make_unique<YYMMDD>("?/?"));
  b5.initialize_from_str("30?/?05?/?55");
  BaseDate tb5 = BaseDate(31, 5, 2030);
  tb5.set_formatter(std::make_unique<YYMMDD>("?/?"));
  CHECK(b5 == tb5);

  BaseDate b6 = BaseDate();
  b6.set_formatter(std::make_unique<MDY>(" "));
  b6.initialize_from_str("4 20 69");
  BaseDate tb6 = BaseDate(20, 4, 2069);
  tb6.set_formatter(std::make_unique<MDY>(" "));
  CHECK(b6 == tb6);
}
