#include "catch.hpp"
#include "date/base_date.hpp"
#include "date/format_date.hpp"


TEST_CASE("Date format")
{
  BaseDate b1 = BaseDate(-4, 2, 2023);
  REQUIRE(b1.to_str() == "01-02-2023");

  BaseDate b2 = BaseDate(50, 2, 2024);
  REQUIRE(b2.to_str() == "29-02-2024");

  BaseDate b3 = BaseDate(30, 2, -99);
  REQUIRE(b3.to_str() == "29-02-1900");

  BaseDate b4 = BaseDate(40, 15, -99);
  REQUIRE(b4.to_str() == "31-12-1900");

  BaseDate b5 = BaseDate(40, 0, -99);
  REQUIRE(b5.to_str() == "31-01-1900");
}


TEST_CASE("Converting days <-> date")
{
  BaseDate b1 = BaseDate(23, 10, 2023);
  REQUIRE(BaseDate::date_to_days(b1) == 45220);

  BaseDate b2 = BaseDate(28, 3, 2063);
  REQUIRE(BaseDate::date_to_days(b2) == 59621);

  BaseDate b3 = BaseDate::days_to_date(45220);
  REQUIRE(b3.to_str() == "23-10-2023");

  BaseDate b4 = BaseDate::days_to_date(59621);
  REQUIRE(b4.to_str() == "28-03-2063");
}


TEST_CASE("Adding days to date")
{
  BaseDate b1 = BaseDate(5, 12, 2001);
  b1.add_days(100);
  REQUIRE(b1.to_str() == "15-03-2002");

  BaseDate b2 = BaseDate(5, 12, 2019);
  b2.add_days(100);
  REQUIRE(b2.to_str() == "14-03-2020");

  BaseDate b3 = BaseDate(16, 4, 2010);
  b3.add_days(365);
  REQUIRE(b3.to_str() == "16-04-2011");

  BaseDate b4 = BaseDate(16, 4, 2007);
  b4.add_days(365);
  REQUIRE(b4.to_str() == "15-04-2008");

  BaseDate b5 = BaseDate(15, 3, 2002);
  b5.add_days(-100);
  REQUIRE(b5.to_str() == "05-12-2001");

  BaseDate b6 = BaseDate(14, 3, 2020);
  b6.add_days(-100);
  REQUIRE(b6.to_str() == "05-12-2019");

  BaseDate b7 = BaseDate(16, 4, 2011);
  b7.add_days(-365);
  REQUIRE(b7.to_str() == "16-04-2010");

  BaseDate b8 = BaseDate(15, 4, 2008);
  b8.add_days(-365);
  REQUIRE(b8.to_str() == "16-04-2007");

  BaseDate b9 = BaseDate(2, 1, 1900);
  b9.add_days(-365);
  REQUIRE(b9.to_str() == "01-01-1900");

  BaseDate b10 = BaseDate(10, 1, 2001);
  b10.add_days(2000);
  REQUIRE(b10.to_str() == "03-07-2006");

  BaseDate b11 = BaseDate(3, 7, 2006);
  b11.add_days(-2000);
  REQUIRE(b11.to_str() == "10-01-2001");
}


TEST_CASE("Adding months to date")
{
  BaseDate b1 = BaseDate(5, 5, 2024);
  b1.add_months(1);
  REQUIRE(b1.to_str() == "05-06-2024");

  BaseDate b2 = BaseDate(31, 1, 2024);
  b2.add_months(1);
  REQUIRE(b2.to_str() == "29-02-2024");

  BaseDate b3 = BaseDate(31, 1, 2024);
  b3.add_months(12);
  REQUIRE(b3.to_str() == "31-01-2025");

  BaseDate b4 = BaseDate(31, 1, 2024);
  b4.add_months(13);
  REQUIRE(b4.to_str() == "28-02-2025");

  BaseDate b5 = BaseDate(1, 2, 2001);
  b5.add_months(22);
  REQUIRE(b5.to_str() == "01-12-2002");

  BaseDate b6 = BaseDate(1, 2, 2001);
  b6.add_months(100);
  REQUIRE(b6.to_str() == "01-06-2009");

  BaseDate b7 = BaseDate(31, 1, 2024);
  b7.add_months(-12);
  REQUIRE(b7.to_str() == "31-01-2023");

  BaseDate b8 = BaseDate(29, 2, 2024);
  b8.add_months(-13);
  REQUIRE(b8.to_str() == "29-01-2023");

  BaseDate b9 = BaseDate(1, 12, 2002);
  b9.add_months(-22);
  REQUIRE(b9.to_str() == "01-02-2001");

  BaseDate b10 = BaseDate(5, 5, 2024);
  b10.add_months(-1);
  REQUIRE(b10.to_str() == "05-04-2024");

  BaseDate b11 = BaseDate(31, 3, 2024);
  b11.add_months(-1);
  REQUIRE(b11.to_str() == "29-02-2024");

  BaseDate b12 = BaseDate(1, 6, 2009);
  b12.add_months(-100);
  REQUIRE(b12.to_str() == "01-02-2001");
}


TEST_CASE("Adding years to date")
{
  BaseDate b1 = BaseDate(29, 2, 2000);
  b1.add_years(1);
  REQUIRE(b1.to_str() == "28-02-2001");

  BaseDate b2 = BaseDate(29, 2, 2000);
  b2.add_years(-10000);
  REQUIRE(b2.to_str() == "01-01-1900");

  BaseDate b3 = BaseDate(29, 2, 2000);
  b3.add_years(10000);
  REQUIRE(b3.to_str() == "31-12-9999");
}


TEST_CASE("Displaying different formats of date")
{
  BaseDate b1 = BaseDate(1, 4, 2001);
  REQUIRE(b1.to_str() == "01-04-2001");

  BaseDate b2 = BaseDate(29, 5, 2002);
  b2.set_formatter(std::make_unique<MMDDYYYY>());
  REQUIRE(b2.to_str() == "05-29-2002");

  BaseDate b3 = BaseDate(9, 5, 2002);
  b3.set_formatter(std::make_unique<DDMMYY>());
  REQUIRE(b3.to_str() == "09-05-02");

  BaseDate b4 = BaseDate(9, 5, 2002);
  b4.set_formatter(std::make_unique<MMDDYY>());
  REQUIRE(b4.to_str() == "05-09-02");

  BaseDate b5 = BaseDate(9, 5, 2002);
  b5.set_formatter(std::make_unique<MMDDYY>("/"));
  REQUIRE(b5.to_str() == "05/09/02");

  BaseDate b6 = BaseDate(9, 5, 2002);
  b6.set_formatter(std::make_unique<YYYYMMDD>("#"));
  REQUIRE(b6.to_str() == "2002#05#09");

  BaseDate b7 = BaseDate(9, 5, 2002);
  b7.set_formatter(std::make_unique<DMY>(""));
  REQUIRE(b7.to_str() == "9502");

  BaseDate b8 = BaseDate(9, 5, 2002);
  b8.set_formatter(std::make_unique<MDY>(""));
  REQUIRE(b8.to_str() == "5902");

  BaseDate b9 = BaseDate(9, 5, 2002);
  b9.set_formatter(std::make_unique<MonthDY>("dsad"));
  REQUIRE(b9.to_str() == "May 9, 2002");

  BaseDate b10 = BaseDate(9, 5, 2002);
  b10.set_formatter(std::make_unique<Roman>());
  REQUIRE(b10.to_str() == "9 V 2002");

  BaseDate b11 = BaseDate(99, 2, 1800);
  b11.set_formatter(std::make_unique<DDMMYY>("."));
  REQUIRE(b11.to_str() == "29.02.00");
}


TEST_CASE("Day of week from date")
{
  BaseDate b1 = BaseDate(1, 7, 1902);
  REQUIRE(b1.day_of_week() == 2);

  BaseDate b2 = BaseDate(1, 7, 1902);
  REQUIRE(b2.day_of_week_name() == "Tuesday");

  BaseDate b3 = BaseDate(16, 4, 2010);
  REQUIRE(b3.day_of_week() == 5);

  BaseDate b4 = BaseDate(16, 4, 2010);
  REQUIRE(b4.day_of_week_name() == "Friday");

  BaseDate b5 = BaseDate(14, 9, 3989);
  REQUIRE(b5.day_of_week() == 4);

  BaseDate b6 = BaseDate(14, 9, 3989);
  REQUIRE(b6.day_of_week_name() == "Thursday");

  BaseDate b7 = BaseDate(15, 8, 2050);
  REQUIRE(b7.day_of_week() == 1);

  BaseDate b8 = BaseDate(15, 8, 2050);
  REQUIRE(b8.day_of_week_name() == "Monday");

  BaseDate b9 = BaseDate(3, 11, 2001);
  REQUIRE(b9.day_of_week() == 6);

  BaseDate b10 = BaseDate(3, 11, 2001);
  REQUIRE(b10.day_of_week_name() == "Saturday");
}


// TEST_CASE("Current time related")
// {
//   BaseDate b1 = BaseDate::today();
//   REQUIRE(b1.to_str() == "01-02-2024");

//   BaseDate b2 = BaseDate(1, 2, 2024);
//   REQUIRE(b2.remaining_days() == 0);

//   BaseDate b3 = BaseDate(31, 1, 2024);
//   REQUIRE(b3.remaining_days() == -1);

//   BaseDate b4 = BaseDate(15, 2, 2024);
//   REQUIRE(b4.remaining_days() == 14);
// }

