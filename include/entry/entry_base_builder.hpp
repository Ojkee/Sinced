#ifndef ENTRY_BASE_BUILDER_HPP
#define ENTRY_BASE_BUILDER_HPP

#include "entry_base.hpp"
#include <cstdint>
#include <string>

class EntryBuilder {
public:
  EntryBuilder() = default;
  EntryBuilder(const EntryBuilder &builder) = delete;
  EntryBuilder(const uint32_t &last_id_) : last_id(last_id_){};
  virtual ~EntryBuilder() = default;

  void inline set_last_id(const uint32_t &last_id_) { last_id = last_id_; }
  [[nodiscard]] uint32_t inline get_last_id() const { return last_id; }

protected:
  uint32_t last_id{};
};

class EntryTaskBuilder : public EntryBuilder {
public:
  EntryTaskBuilder() = default;
  [[nodiscard]] EntryTask get(const std::string &content);
  [[nodiscard]] EntryTask *add_deadline(EntryTask &task,
                                        std::unique_ptr<BaseDate> deadline);
  [[nodiscard]] EntryTask *add_recursive_days(EntryTask &task,
                                              const std::string &d);
  [[nodiscard]] EntryTask *add_recursive_months(EntryTask &task,
                                                const std::string &m);
  [[nodiscard]] EntryTask *add_recursive_years(EntryTask &task,
                                               const std::string &y);
};

#endif // ENTRY_BASE_BUILDER_HPP
