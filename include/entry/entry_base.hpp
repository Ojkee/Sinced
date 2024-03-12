#ifndef BASE_ENTRY_HPP
#define BASE_ENTRY_HPP

#include "../date/base_date.hpp"
#include "../entry/entry_formatter.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

enum class Status { ongoing, done, canceled, undetermined };

class EntryBase {
public:
  EntryBase(){};
  EntryBase(const std::string &line);
  virtual void tokenize(const std::string &line) = 0;
  virtual std::string info() const = 0;
  virtual operator std::string() const = 0;
  virtual ~EntryBase() = default;
  [[nodiscard]] static std::vector<std::string>
  line_to_tokens(const std::string &line);

  [[nodiscard]] std::string get_id() const { return id; }
  [[nodiscard]] std::string get_content() const { return content; }

  const EntryInfoFormatter &get_info_formatter() const {
    return *info_formatter;
  }
  void set_info_formatter(std::unique_ptr<EntryInfoFormatter> &&_formatter) {
    info_formatter = std::move(_formatter);
  }

protected:
  std::string id{};
  std::string content{};

  std::unique_ptr<EntryInfoFormatter> info_formatter = nullptr;
};

class EntryTask : public EntryBase {
public:
  EntryTask() = delete;
  EntryTask(const std::string &line) { tokenize(line); }
  void tokenize(const std::string &line) override;
  operator std::string() const override;
  std::string info() const override;
  [[nodiscard]] inline bool is_repetetive() const {
    return r_days || r_months || r_years;
  };
  [[nodiscard]] bool
  is_in_timeframe(const std::unique_ptr<BaseDate> &date_) const;
  [[nodiscard]] BaseDate next_repetetive_deadline() const;

  [[nodiscard]] inline Status get_status() const { return status; }
  [[nodiscard]] inline const std::unique_ptr<BaseDate> &get_deadline() const {
    return deadline;
  }

protected:
  std::unique_ptr<BaseDate> deadline = nullptr;
  Status status = Status::undetermined;
  uint16_t r_days{};
  uint16_t r_months{};
  uint16_t r_years{};
};

class EntryCategory : public EntryBase {
public:
  EntryCategory() = delete;
  EntryCategory(const std::string &line) { tokenize(line); }
  void tokenize(const std::string &line) override;
  operator std::string() const override;
  std::string info() const override;
};

class EntryRelation : public EntryBase {
public:
  EntryRelation() = delete;
  EntryRelation(const std::string &line) { tokenize(line); }
  void tokenize(const std::string &line) override;
  operator std::string() const override;
  std::string info() const override;

  [[nodiscard]] std::string get_content_category() const {
    return content_category;
  }

private:
  std::string content_category;
};

#endif // BASE_ENTRY_HPP
