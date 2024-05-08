## ADD

- [x] Adding new task
```
scd add "New task"
```
- [x] Adding category
```
scd add @"New category"
```
- [x] Task to catogory
```
scd add "Already existing task" @"Category"
```
- [x] New task directly to catogory
```
scd add "New Task" @"Category"
```
- [x] New task with deadline
```
scd add "New task" 04-20-2069
```
- [x] New task with parameter
```
scd add "New task" -wr
```
- [x] New task with parameter and option
```
scd add "New task" -w 2
```
- [x] New task with parameter and option recursive
```
scd add "New task" -wr 2
```
- [x] New task with deadline and parameter recursive
```
scd add "New task" 04-20-2069 -wr
```
- [ ] New task with deadline directly into Category
```
scd add "New task" @"Category" 20-04-2026
```
- [ ] New task with deadline, parameters directly into Category
```
scd add "New task" @"Category" 20-04-2069 -y
```
- [x] New task with deadline, parameters, option recursive directly into Category
```
scd add "New task" @"Category" 20-04-2069 -yr 5
```
- [x] New task with deadline, parameter and option
```
acd add "New task" 04-20-2069 -m 3
```
