#ifndef HELP_MESSAGES_HPP_
#define HELP_MESSAGES_HPP_

#include <string>

class Help_Messages {
 public:
  [[nodiscard]] constexpr static inline std::string general() {
    return "- add - adds task or @category\n"
           "- log - logs task or @category\n"
           "- mod - modifies task name / -parameter / @category membership\n"
           "- rm - removes task/task from @category / @category / tasks with "
           "@category\n"
           "- set - modifies settings such as date format / separator in date "
           " / tasks filter\n"
           "- reset - resets settings or whole database"
           "- help - displays additional information about above commands\n";
  }
  [[nodiscard]] constexpr static inline std::string add() {
    return
        R"(
NAME
    scd add - Adding new tasks, categories, and assigning tasks to categories

SYNOPSIS
    scd add "New task"
    scd add @"New category"
    scd add "Existing task" @"Category"
    scd add "New task" @"Category"
    scd add "New task" MM-DD-YYYY
    scd add "New task" -wr
    scd add "New task" -w N
    scd add "New task" -wr N
    scd add "New task" MM-DD-YYYY -wr
    scd add "New task" @"Category" MM-DD-YYYY
    scd add "New task" @"Category" MM-DD-YYYY -yr N
    scd add "New task" MM-DD-YYYY -m N

DESCRIPTION
    The `scd add` command allows you to:

    - Add a new task.
    - Add a new category.
    - Assign an existing task to a category.
    - Add a new task directly to a category.
    - Add a new task with a deadline.
    - Add a new task with parameters and options.

OPTIONS
    "New task"
        Adds a new task.

    @"New category"
        Adds a new category.

    "Existing task" @"Category"
        Assigns an existing task to a category.

    "New task" @"Category"
        Adds new task and puts directly to category.

    MM-DD-YYYY
        Adds a new task with a deadline.

    -[d/w/m/y]r
        Adds task that has deadline every day/week/month/year.

    -[d/w/m/y] N
        Adds task with deadline in N days/weeks/months/years.

    -[d/w/m/y]r N
        Adds task with deadline every N days/weeks/months/years.

    MM-DD-YYYY -[d/w/m/y]
        Adds a new task with a deadline that repeats every day/week/month/year.

    Every task with parameters, deadlines etc. can be placed directly into category.
    Every deadline can be repeated and has 'N' multiplyer.

EXAMPLES
    scd add "Finish report"
    scd add @"Work Projects"
    scd add "Finish report" @"Work Projects"
    scd add "Plan vacation" @"Personal"
    scd add "Renew subscription" 12-31-2024
    scd add "Update software" -wr
    scd add "Check emails" -w 5
    scd add "Clean house" -mr 7
    scd add "Submit assignment" 10-15-2023 -wr
    scd add "Meeting with team" @"Work" 05-12-2024
    scd add "Project deadline" @"Projects" 09-30-2025 -yr 2
    scd add "Doctor appointment" 04-20-2069 -m 3

)";
  }
  [[nodiscard]] constexpr static inline std::string add_syntax() {
    return "\tscd add <task / @category>\n\tscd add <task> <@category> "
           "<-parameter> [parameter flags]";
  }
  [[nodiscard]] constexpr static inline std::string log() {
    return
        R"(
NAME
    scd log - Display tasks and categories with various filtering options

SYNOPSIS
    scd log "task_name"
    scd log @"category"
    scd log -a
    scd log -ac
    scd log -deadline MM-DD-YYYY
    scd log -status Status

DESCRIPTION
    The `scd log` command allows you to:

    - Display details of a specific task.
    - Display details of tasks in a specific category.
    - Display all tasks.
    - Discplays all categories.
    - Filter tasks by deadline.
    - Filter tasks by status.

OPTIONS
    "task_name"
        Displays details of the specified task.

    @"category"
        Displays details of tasks in the specified category.

    -a
        Displays all tasks.

    -ac
        Displays all categories.

    -deadline MM-DD-YYYY
        Filters tasks by the specified deadline.

    -status Status
        Filters tasks by the specified status (e.g., Ongoing, Completed, Done).

FILTERS
    - deafault
    - deadline
    - category
    - status

EXAMPLES
    scd log "Finish report"
    scd log @"Work Projects"
    scd log -a
    scd log -ac
    scd log -deadline 12-31-2024
    scd log -status Ongoing

)";
  }
  [[nodiscard]] constexpr static inline std::string log_syntax() {
    return "\tscd log <task / @category>\n\tscd log <task> <-parameter> "
           "[parameter flags]";
  }
  [[nodiscard]] constexpr static inline std::string mod() {
    return
        R"(
NAME
    scd mod - Modify tasks and categories

SYNOPSIS
    scd mod @"category" -name @"new_category_name"
    scd mod "task_name" -name "new_task_name"
    scd mod "task_name" -status Status
    scd mod "task_name" -deadline MM.DD.YYYY
    scd mod "task_name" -r d7
    scd mod "task_name" -to @"category"
    scd mod "task_name" -x

DESCRIPTION
    The `scd mod` command allows you to:

    - Modify the name of a category.
    - Modify the name of a task.
    - Modify the status of a task.
    - Modify the deadline of a task.
    - Modify recursive parameters of a task.
    - Move a task to another category.
    - Exclude a task from any category.

OPTIONS
    @"category" -name @"new_category_name"
        Modifies the name of the category to the new name.

    "task_name" -name "new_task_name"
        Modifies the name of the task to the new name.

    "task_name" -status Status
        Modifies the status of the task (e.g., Ongoing).

    "task_name" -deadline MM.DD.YYYY
        Modifies the deadline of the task.

    "task_name" -r d5w9...
        Modifies the recursive parameters of the task.

    "task_name" -to @"category"
        Moves the task to another category.

    "task_name" -x
        Excludes the task from any category.

EXAMPLES
    scd mod @"Work Projects" -name @"Office Projects"
    scd mod "Finish report" -name "Complete report"
    scd mod "Finish report" -status Ongoing
    scd mod "Finish report" -deadline 12.31.2024
    scd mod "Finish report" -r d2w1
    scd mod "Finish report" -to @"Work Projects"
    scd mod "Finish report" -x

)";
  }
  [[nodiscard]] constexpr static inline std::string mod_syntax() {
    return "scd mod <task / @category> <-parameter> <parameter value>";
  }
  [[nodiscard]] constexpr static inline std::string rm() {
    return
        R"(
NAME
    scd rm - Remove tasks and categories

SYNOPSIS
    scd rm "task_name"
    scd rm "@category_name"
    scd rm "@category_name" -a

DESCRIPTION
    The `scd rm` command allows you to:

    - Remove a task.
    - Remove a category.
    - Remove a category along with all its tasks.

OPTIONS
    "task_name"
        Removes the specified task.

    "@category_name"
        Removes the specified category.

    "@category_name" -a
        Removes the specified category along with all its tasks.

EXAMPLES
    scd rm "Finish report"
    scd rm @"Work Projects"
    scd rm @"Work Projects" -a

)";
  }
  [[nodiscard]] constexpr static inline std::string rm_syntax() {
    return "scd rm <task / @category> [-a]";
  }
  [[nodiscard]] constexpr static inline std::string set() {
    return
        R"(
NAME
    scd set - Configure settings for date format, separator, and sorter

SYNOPSIS
    scd set -df YYYYMMDD
    scd set -ds .
    scd set -s deadline

DESCRIPTION
    The `scd set` command allows you to:

    - Set the date format.
    - Set the separator for the date format.
    - Set the sorter for tasks.

OPTIONS
    -df YYYYMMDD
        Sets the date format to the specified format

    -ds .
        Sets the separator for the date format (e.g., -).

    -s sorter
        Sets the sorter for tasks 

DATE FORMATS
    - DDMMYYYY
    - DDMMYY
    - DMY
    - MMDDYYYY
    - MMDDYY
    - MDY
    - YYYYMMDD
    - YYMMDD

SORTERS
    - default
    - deadline
    - status
    - alphabetical

STATUS
    - Ongoing
    - Done
    - Cancel

EXAMPLES
    scd set -df YYYYMMDD
    scd set -ds .
    scd set -s deadline

)";
  }
  [[nodiscard]] constexpr static inline std::string set_syntax() {
    return "scd set <-parameter> <parameter flag>";
  }
  [[nodiscard]] constexpr static inline std::string reset() {
    return
        R"(
NAME
    scd reset - Reset settings and database

SYNOPSIS
    scd reset -default
    scd reset -force

DESCRIPTION
    The `scd reset` command is used to reset the application's settings and database.

OPTIONS
    -default
        Resets the application to its default settings. This option will not delete any data in the database.

    -force
        Resets the application to its default settings and deletes the entire database. Use this option with caution as all data will be lost.

EXAMPLES
    Reset the application to default settings:
        scd reset -settings

    Reset the entire database:
        scd reset -database

    Reset the application to default settings and delete the entire database:
        scd reset -forceall
)";
  }
  [[nodiscard]] constexpr static inline std::string reset_syntax() {
    return "scd reset <parameter>";
  }
  [[nodiscard]] constexpr static inline std::string help() { return ""; }
  [[nodiscard]] constexpr static inline std::string help_syntax() {
    return "scd help <command>";
  }

 private:
  const std::string GREEN = "\033[32m";
  const std::string RESET_COLOR = "\033[0m";

  const std::string BOLD = "\x1b[1m";
  const std::string UNBOLD = "\x1b[0m";
};

#endif  // !HELP_MESSAGES_HPP_
