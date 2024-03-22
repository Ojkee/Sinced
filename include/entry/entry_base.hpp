#ifndef ENTRY_BASE_HPP
#define ENTRY_BASE_HPP

#include "../date/base_date.hpp"
#include "../entry/entry_formatter.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

enum class Status { ongoing, done, canceled, undetermined };

class EntryBase {
public:
  EntryBase() = default;
  EntryBase(const std::string &line);
  virtual void tokenize(const std::string &) = 0;
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
  EntryTask() = default;
  EntryTask(const std::string &line) { tokenize(line); }
  void tokenize(const std::string &line) override;
  operator std::string() const override;
  std::string info() const override;
  [[nodiscard]] inline bool is_repetetive() const {
    return r_days || r_months || r_years;
  };
  [[nodiscard]] bool is_in_timeframe(const BaseDate &date_) const;
  [[nodiscard]] BaseDate next_repetetive_deadline() const;

  [[nodiscard]] inline Status get_status() const { return status; }
  void inline set_status(const Status &status_) { status = status_; }
  [[nodiscard]] inline const std::optional<BaseDate> get_deadline() const {
    return (deadline) ? deadline : std::nullopt;
  }
  void inline set_deadline(const BaseDate &deadline_) { deadline = deadline_; }
  void inline set_recursive(const uint16_t &d, const uint16_t &m,
                            const uint16_t &y) {
    r_days = d;
    r_months = m;
    r_years = y;
  }

  class Builder {
  public:
    Builder() : task(std::make_unique<EntryTask>()) {}

    Builder &add_id(const std::string &id_) {
      task->id = id_;
      return *this;
    }
    Builder &add_content(const std::string &content_) {
      task->content = content_;
      return *this;
    }
    Builder &add_status(const Status &status_) {
      task->status = status_;
      return *this;
    }
    Builder &add_deadline(const BaseDate &deadline_) {
      task->set_deadline(deadline_);
      return *this;
    }
    Builder &add_recursive_days(const uint16_t &rdays) {
      task->set_recursive(rdays, 0, 0);
      return *this;
    }
    Builder &add_recursive_months(const uint16_t &rmonths) {
      task->set_recursive(0, rmonths, 0);
      return *this;
    }
    Builder &add_recursive_years(const uint16_t &ryears) {
      task->set_recursive(0, 0, ryears);
      return *this;
    }
    std::shared_ptr<EntryTask> get() { return std::move(task); }

  private:
    std::unique_ptr<EntryTask> task;
  };

protected:
  std::optional<BaseDate> deadline;
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

#endif // ENTRY_BASE_HPP
