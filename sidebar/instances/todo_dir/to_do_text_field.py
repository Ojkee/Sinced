import flet as ft

from settings.enums import Color
from settings.config_init import cg


class To_Do_Text_Field(ft.TextField):
    def __init__(self, on_change, add_task):
        super().__init__(
            label="new todos",
            label_style=ft.TextStyle(
                color=cg.get_color(Color.DARK),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
            ),
            hint_text="awesome task",
            hint_style=ft.TextStyle(
                color=cg.get_color(Color.DARK),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
                italic=True,
            ),
            text_style=ft.TextStyle(
                color=cg.get_color(Color.DARK),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
            ),
            expand=True,
            border=ft.InputBorder.UNDERLINE,
            border_color=cg.get_color(Color.DARK),
            cursor_color=cg.get_color(Color.DARK),
            content_padding=10,
            on_change=on_change,
            on_submit=add_task
        )


    def is_empty(self) -> bool:
        return self.value == "" or not self.value.strip()

    def reset(self) -> None:
        self.value = ""
        self.update()

    def get_normalized_task_text(self) -> str:
        return self.value.strip()


