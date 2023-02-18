import flet as ft
from datetime import datetime
from dataclasses import dataclass

from typing import Union
from settings.config_init import cg
from settings.enums import Color, Date_Format


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
        self.task_data = Task(t_id, text, date_added, deadline)
        self.text_size = 16

        self.text_info = self.build_task_info()

        self.main_task_row = ft.Row(
            controls=[
                self.text_info,
            ],
        )

        super().__init__(
            content=self.main_task_row,
        )

    def __repr__(self) -> str:
        return f"""{self.task_data.id} | "{self.task_data.text}"\nCreated: {self.task_data.date_added}\nDeadline: {self.task_data.deadline}"""

    @property
    def deadline(self) -> Union[str, None]:
        return self.task_data.deadline

    @deadline.setter
    def deadline(self, val: Union[str, None]):
        self.task_data.deadline = val


    @property
    def id(self) -> int:
        return self.task_data.id

    @id.setter
    def id(self, val: int):
        self.task_data.id = val


    @property
    def text(self) -> str:
        return self.task_data.text

    @text.setter
    def text(self, val: str):
        self.task_data.text = val


    @property
    def is_checked(self) -> bool:
        return self.is_checked


    def build_task_info(self) -> ft.Container:
        remaining = "" if self.task_data.deadline is None else self.get_remaining_time_to_deadline(self.task_data.deadline)
        main_row = ft.Row(
            controls=[
                ft.Column(
                    controls=[
                        ft.Text(
                            value=self.task_data.text,
                            color=cg.get_color(Color.DARK),
                            font_family=cg.font(),
                            weight=ft.FontWeight.W_700,
                            size=self.text_size,
                        ),
                    ],
                    width=cg.sidebar_width * 3 // 4,
                    horizontal_alignment=ft.CrossAxisAlignment.START,
                ),
                ft.Column(
                    controls=[
                        ft.Text(
                            value=remaining,
                            color=cg.get_color(Color.DARK),
                            font_family=cg.font(),
                            weight=ft.FontWeight.W_700,
                            size=self.text_size // 6 * 5,
                        ),
                    ],
                    width=cg.sidebar_width // 4,
                    horizontal_alignment=ft.CrossAxisAlignment.CENTER,
                    alignment=ft.MainAxisAlignment.CENTER
                )
            ],
            width=cg.sidebar_width,
            spacing=10
        )
        return ft.Container(
            content=main_row,
            border_radius=10,
            bgcolor=cg.get_color(Color.LIGHT_2),
            on_click=None
        )


    @staticmethod
    def get_remaining_time_to_deadline(date_text: str) -> str:
        now = datetime.now()
        delta = datetime.strptime(date_text, Date_Format.DD_MM_YYYY.value) - now
        return f"{delta.days}\ndays"


