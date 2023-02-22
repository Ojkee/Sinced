import flet as ft
from typing import Union
from datetime import datetime

from settings.config_init import cg
from settings.enums import Date_Format
from sidebar.instances.todo_dir.task_container import Task_Container


class Task_List(ft.Column):
    Tasks_Added_Counter = 0

    def __init__(self):
        super().__init__(
            controls=[

            ],
            width=cg.sidebar_width,
            height=cg.window_height * 4 // 5,
            scroll=ft.ScrollMode.ALWAYS,
            spacing=6
        )


    def add_task(self, text: str, deadline: Union[str, None]) -> None:
        new_task = Task_Container(
            t_id=Task_List.Tasks_Added_Counter,
            text=text,
            date_added=datetime.now().strftime(Date_Format.DD_MM_YYYY.value),
            deadline=deadline,
        )
        self.controls.append(new_task)
        Task_List.Tasks_Added_Counter += 1


