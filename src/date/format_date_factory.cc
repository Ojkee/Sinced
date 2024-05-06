#include "../../include/date/format_date_factory.hpp"

#include "../../include/date/format_date.hpp"

std::shared_ptr<FormatDate> FormatDateFactory::get(
    const std::string &format_date_str, const std::string &separator) noexcept {
  auto it = date_formats_map.find(format_date_str);
  if (it != date_formats_map.end()) {
    it->second->set_separator(separator);
    return it->second;
  }
  return nullptr;
}

bool FormatDateFactory::is_valid_format(
    const std::string &format_date_str) const noexcept {
  auto it = date_formats_map.find(format_date_str);
  return it != date_formats_map.end();
}
