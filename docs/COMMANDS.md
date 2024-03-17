# Syntax
```
  scd <command> <subject> [optional flags with parameters]
```

***Example:***
```
  scd add "My awesome task" -rw 4
```

## Commands
- **add** - adds tasks to task list and @categories to category list
- **rm** - removes task by name or id (if number provided)
- **list** - lists task via name/id/category
- **status** - modifies status of task or whole category

## Subjects

### Task
some todo task, name containing space should be written in quotation marks
```
  scd <command> "new task"
  scd <command> new_task
```

##### Additional flags
- **@category** - \<command\> directly into/from @category

  ```
  scd add "new task" @"category name"
  ```

- **-d / -w / -m \<number\>** - deadline in day/week/month \*number

  ```
  scd add "new task" @"category name" -w   //Task in category with deadline in 7 days
  scd add "new task" -w 5                  //Task with deadline today + 5 * 7 days
  ```

- **-rd / -rw / -rm \<number\>** - same that above but task is repetetive
  ```
  scd add "new task" @1 -rm                //Task in category "1" every month 
  scd add "new task" -rw 5                 //Task with deadline every 5 weeks
  ```

- **date** - adds task with deadline on specific date
```
  scd add task 07-05-2034
  scd add task 2034.05.07
```


### Category
tasks can be categorized info folder-like categories with '@' at the beggining of the name.
```
 scd <command> @"category name"
 scd <command> @category_name
```


### Configure
```
  scd set YYYYMMDD -i                       // date format date in input to program
  scd set YYYYMMDD -o                       // date format that program displays
  scd set -s'sep'                           // separator in date format in 
                                            (TODO: POSSIBLE BETTER SYNTAX IDEA)
```


### List
- scd list
- scd list -a
- scd list @category
- scd list -s
- scd list -s 10
- scd list -w 2

