#ifndef INTERPTERE_HPP
#define INTERPTERE_HPP

#include "../entry/entry_handler.hpp"
#include "token.hpp"

#include <string>
#include <vector>

class Interpreter {
public:
  Interpreter() {}
  Interpreter(const std::string &tasks_path_,
              const std::string &categories_path_,
              const std::string &relations_path_)
      : tasks_path(tasks_path_), categories_path(categories_path_),
        relations_path(relations_path_){};
  void parse(const std::string &user_input) const;

private:
  const EntryHandler entry_handler = EntryHandler();

  std::string tasks_path = "../../records/database/tasks.mdb";
  std::string categories_path = "../../records/database/categoris.mdb";
  std::string relations_path = "../../records/database/relations.mdb";

  void add_commend(const std::vector<Token> &tokens) const;
};

#endif // INTERPTERE_HPP
