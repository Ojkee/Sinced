import flet as ft
from typing import Union
from datetime import datetime

from settings.config_init import cg
from settings.enums import Date_Format

from sidebar.instances.todo_dir.task_list.task_entity.task_dataclass import Task
from sidebar.instances.todo_dir.task_list.on_going_task_list import On_Going_Task_List
from sidebar.instances.todo_dir.task_list.done_task_list import Done_Task_list




class Task_List_Manager(ft.Container):
    Tasks_Added_Counter = 0

    def __init__(self):
        self.current_list_index: int = 0

        self.main_stack = ft.Stack(
            controls=[
                On_Going_Task_List(is_opened=True),
                Done_Task_list(is_opened=False),
            ],
        )

        super().__init__(
            content=self.main_stack,
            width=cg.sidebar_width,
            height=cg.window_height,
        )


    def add_new_task(self, text: str, deadline: Union[str, None]) -> None:
        new_task = Task(
            id=Task_List_Manager.Tasks_Added_Counter,
            text=text,
            date_added=datetime.now().strftime(Date_Format.DD_MM_YYYY.value),
            deadline=deadline,
        )
        self.get_current_list().add_task_to_list(new_task)
        Task_List_Manager.Tasks_Added_Counter += 1


    def get_current_list(self):
        return self.main_stack.controls[self.current_list_index]

