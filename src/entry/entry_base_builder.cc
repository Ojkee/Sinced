#include "../../include/entry/entry_base_builder.hpp"
#include <cstdint>
#include <string>

EntryTask EntryTaskBuilder::get(const std::string &content) {
  const std::string id_str = std::to_string(last_id++);
  return EntryTask(id_str + " " + content + " 0 -1 0 0 0");
}

EntryTask *EntryTaskBuilder::add_recursive_days(EntryTask &task,
                                                const std::string &d) {
  const uint16_t rd = static_cast<uint16_t>(std::stoi(d));
  task.set_recursive(rd, 0, 0);
  return &task;
}

EntryTask *EntryTaskBuilder::add_recursive_months(EntryTask &task,
                                                  const std::string &m) {
  const uint16_t rm = static_cast<uint16_t>(std::stoi(m));
  task.set_recursive(0, rm, 0);
  return &task;
}

EntryTask *EntryTaskBuilder::add_recursive_years(EntryTask &task,
                                                 const std::string &y) {
  const uint16_t ry = static_cast<uint16_t>(std::stoi(y));
  task.set_recursive(0, 0, ry);
  return &task;
}