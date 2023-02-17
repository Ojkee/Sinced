import flet as ft
from typing import Union
from datetime import datetime

from settings.config_init import cg
from sidebar.instances.todo_dir.task import Task


class Task_List(ft.Column):
    Tasks_Added_Counter = 0

    def __init__(self):
        super().__init__(
            controls=[

            ],
            width=cg.sidebar_width,
            scroll=ft.ScrollMode.ALWAYS
        )


    def add_task(self, text: str, deadline: Union[str, None]) -> None:
        self.controls.append(
            Task(
                text=text,
                date_added=datetime.now().strftime("%d-%m-%Y"),
                t_id=Task_List.Tasks_Added_Counter,
                deadline=deadline,
            )
        )
        Task_List.Tasks_Added_Counter += 1


