# Syntax
```
  scd [command] [subject] [additional flas with parameters]
```

***Example:***
```
  scd add "My awesome task" -rw
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
  scd [command] "new task"
  scd [command] new_task
```

##### Additional flags
- **@category** - \[command\] directly into/from @category

  ```
  scd add "new task" @"category name"
  ```

- **-d / -w / -m \[number\]** - deadline in day/week/month \*number

  ```
  scd add "new task" @"category name" -w
  scd add "new task" -w 5
  ```

- **-rd / -rw / -rm \[number\]** - same that above but task is repetetive
  ```
  scd add "new task" @1 -rm
  scd add "new task" -rw 5
  ```

- **date** - adds task with deadline on specific date
```
  scd add task 07-05-2034
  scd add task 2034.05.07
```


### Category
tasks can be categorized info folder-like categories with '@' at the beggining of the name.
```
 scd [command] @"category name"
 scd [command] @category_name
```



### list
- scd list
- scd list -a
- scd list @category
- scd list -s
- scd list -s 10
- scd list -w 2


### Configure
- scd set date YYYYMMDD

