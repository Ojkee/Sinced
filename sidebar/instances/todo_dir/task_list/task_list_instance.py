import flet as ft

from settings.config_init import cg

from sidebar.instances.todo_dir.task_list.task_entity.task_dataclass import Task
from sidebar.instances.todo_dir.task_list.task_entity.draggable_task import Draggable_Task


class Task_List_Instance(ft.Container):
    counter: int = 0

    def __init__(self, is_opened: bool):
        self.index: int = Task_List_Instance.counter
        Task_List_Instance.counter += 1
        self.is_opened: bool = is_opened


        self.main_stack = ft.Stack(
            width=cg.get_task_list_width(is_opened=self.is_opened),
            height=cg.sidebar_height,
        )

        super().__init__(
            content=self.main_stack,
            height=cg.window_height,
            on_click=self.clicked,
        )


    def add_task_to_list(self, task: Task) -> None:
        self.main_stack.controls.append(Draggable_Task(task))


    def clicked(self, e):
        if not self.is_opened:
            print("OPEN TO DO IN FILE TASK LIST INTERFACE")


