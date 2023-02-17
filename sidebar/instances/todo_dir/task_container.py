import flet as ft
import datetime
from dataclasses import dataclass

from typing import Union
from settings.config_init import cg
from settings.enums import Color


@dataclass()
class Task:
    id: int
    text: str
    date_added: str
    deadline: str | None = None


class Task_Container(ft.Container):
    def __init__(
            self,
            t_id: int,
            text: str,
            date_added: str,
            deadline: str | None = None
    ):

        self.task = Task(t_id, text, date_added, deadline)
        self.t_is_checked: bool = False

        self.checkbox = ft.Checkbox(
            fill_color=cg.get_color(Color.DARK),
            on_change=self.clicked,
        )
        self.main_task_row = ft.Row(
            controls=[
                self.checkbox,
                ft.Text(
                    value=self.task.text,
                    color=cg.get_color(Color.DARK),
                    font_family=cg.font(),
                    weight=ft.FontWeight.W_700,
                )
            ],
        )

        super().__init__(
            content=self.main_task_row,
            on_click=self.clicked
        )
        print(self)

    def __repr__(self) -> str:
        return f"""{self.task.id} | "{self.task.text}"\nCreated: {self.task.date_added}\nDeadline: {self.task.deadline}"""

    @property
    def deadline(self) -> Union[str, None]:
        return self.task.deadline

    @deadline.setter
    def deadline(self, val: Union[str, None]):
        self.task.deadline = val


    @property
    def id(self) -> int:
        return self.task.id

    @id.setter
    def id(self, val: int):
        self.task.id = val


    @property
    def text(self) -> str:
        return self.task.text

    @text.setter
    def text(self, val: str):
        self.task.text = val


    @property
    def is_checked(self) -> bool:
        return self.is_checked


    @is_checked.setter
    def is_checked(self, val: bool):
        self.t_is_checked = val


    def clicked(self, e):
        self.t_is_checked = not self.t_is_checked
        self.checkbox.value = self.t_is_checked
        self.checkbox.update()
