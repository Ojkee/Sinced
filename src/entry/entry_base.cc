#include <cstddef>
#include <format>
#include <iomanip>
#include <sstream>
#include <vector>

#include "../../include/entry/entry_base.hpp"

[[nodiscard]] std::vector<std::string>
EntryBase::line_to_tokens(const std::string &line) {
  std::vector<std::string> tokens;
  std::istringstream iss(line);
  std::string token;
  while (iss >> std::quoted(token)) {
    tokens.push_back(token);
  }
  return tokens;
}

void EntryTask::tokenize(const std::string &line) {
  auto tokens = line_to_tokens(line);
  id = tokens[0];
  content = tokens[1];
  status = static_cast<Status>(std::stoi(tokens[2]));
  if (tokens[3] != "-1") {
    uint16_t int_date = (uint16_t)std::stoul(tokens[3]);
    deadline = std::make_unique<BaseDate>(BaseDate::days_to_date(int_date));
  }
  r_days = static_cast<uint16_t>(std::stoul(tokens[4]));
  r_months = static_cast<uint16_t>(std::stoul(tokens[5]));
  r_years = static_cast<uint16_t>(std::stoul(tokens[6]));
}

EntryTask::operator std::string() const {
  int date_str = (deadline == nullptr) ? -1 : BaseDate::date_to_days(*deadline);
  return std::format("{} \"{}\" {} {} {} {} {}", id, content,
                     static_cast<uint16_t>(status), date_str, r_days, r_months,
                     r_years);
}

std::string EntryTask::info() const {
  auto status_to_str = [](Status _status) -> std::string {
    switch (_status) {
    case Status::ongoing:
      return "Ongoing";
    case Status::done:
      return "Done";
    case Status::canceled:
      return "Canceled";
    default:
      return "Undetermined";
    }
  };
  std::string status_str = status_to_str(status);
  std::string deadline_str = "";
  std::string repetetive_str = "";
  if (is_repetetive() && status == Status::ongoing) {
    deadline_str = std::string(next_repetetive_deadline());
    repetetive_str += " every: ";
    if (r_years != 0) {
      repetetive_str += std::to_string(r_years) + " year ";
    }
    if (r_months != 0) {
      repetetive_str += std::to_string(r_months) + " month ";
    }
    if (r_days != 0) {
      repetetive_str += std::to_string(r_days) + " day ";
    }
  } else {
    deadline_str = (deadline) ? std::string(*deadline) : "None";
  }
  return std::format("\"{}\"\n\t{}\n\tdeadline: {}", content, status_str,
                     deadline_str + repetetive_str);
}

[[nodiscard]] BaseDate EntryTask::next_repetetive_deadline() const {
  BaseDate today_date = BaseDate::today();
  BaseDate result = BaseDate::shallow_copy_from_unique(deadline);
  for (std::size_t i = 0; i < 200000; ++i) {
    if (result >= today_date) {
      return result;
    }
    result.add_years(r_years);
    result.add_months(r_months);
    result.add_days(r_days);
  }
  return result;
}

void EntryCategory::tokenize(const std::string &line) {
  auto tokens = line_to_tokens(line);
  id = tokens[0];
  content = tokens[1];
}

EntryCategory::operator std::string() const {
  return std::format("{} \"{}\"", id, content);
}

std::string EntryCategory::info() const {
  return std::format("\"{}\"\n\tid: {}", content, id);
}

void EntryRelation::tokenize(const std::string &line) {
  auto tokens = line_to_tokens(line);
  id = tokens[0];
  content = tokens[1];
  content_category = tokens[2];
}

EntryRelation::operator std::string() const {
  return std::format("{} {} {}", id, content, content_category);
}

std::string EntryRelation::info() const { return std::string(*this); }

[[nodiscard]] bool
EntryTask::is_in_timeframe(const std::unique_ptr<BaseDate> &date_) const {
  BaseDate today = BaseDate::today();
  if (is_repetetive()) {
    BaseDate nextDeadline = next_repetetive_deadline();
    return nextDeadline <= *date_;
  }
  return (deadline) ? today <= *deadline && *deadline <= *date_ : false;
}