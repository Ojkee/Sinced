import flet as ft
import datetime

from typing import Union
from settings.config_init import cg
from settings.enums import Color
from bridges.file_saver import Saver



class Task(ft.Container):
    def __init__(
            self,
            text: str,
            date_added: str,
            t_id: int,
            deadline: Union[str, None] = None,
    ):
        self.t_date_added: str = date_added
        self.t_id: int = t_id
        self.t_text: str = text
        self.t_is_checked: bool = False
        self.t_deadline: Union[str, None] = deadline

        self.checkbox = ft.Checkbox(
            fill_color=cg.get_color(Color.DARK),
            on_change=self.clicked,
        )
        self.main_task_row = ft.Row(
            controls=[
                self.checkbox,
                ft.Text(
                    value=text,
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

    @property
    def deadline(self) -> Union[str, None]:
        return self.t_deadline

    @deadline.setter
    def deadline(self, val: Union[str, None]):
        self.t_deadline = val


    @property
    def id(self) -> int:
        return self.t_id

    @id.setter
    def id(self, val: int):
        self.t_id = val


    @property
    def text(self) -> str:
        return self.t_text

    @text.setter
    def text(self, val: str):
        self.t_text = val


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
