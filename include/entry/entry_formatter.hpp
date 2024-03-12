#ifndef ENTRY_FORMATTER_HPP
#define ENTRY_FORMATTER_HPP

class EntryInfoFormatter {
public:
  EntryInfoFormatter(){};
  virtual ~EntryInfoFormatter() = default;
};

class EntryTaskInfoFormatter : public EntryInfoFormatter {};
#endif // !ENTRY_FORMATTER_HPP
