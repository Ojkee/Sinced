import flet as ft
from typing import Union
from datetime import datetime

from settings.config_init import cg
from settings.enums import Color, Date_Format

class Text_Field_Date_Row(ft.Row):
    def __init__(
            self,
            custom_on_submit
    ):
        self.text_field = ft.TextField(
            keyboard_type=ft.KeyboardType.DATETIME,
            label="deadline",
            label_style=ft.TextStyle(
                color=cg.get_color(Color.DARK),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
            ),
            text_style=ft.TextStyle(
                color=cg.get_color(Color.DARK),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
            ),
            hint_text="dd-mm-yyyy",
            hint_style=ft.TextStyle(
                color=cg.get_color(Color.DARK),
                font_family=cg.font(),
                italic=True,
            ),
            border=ft.InputBorder.UNDERLINE,
            border_color=cg.get_color(Color.DARK),
            cursor_color=cg.get_color(Color.DARK),
            content_padding=10,
            on_submit=custom_on_submit
        )

        super().__init__(
            controls=[
                ft.Container(
                    content=ft.Row(
                        controls=[
                            self.text_field,
                        ],
                        width=cg.sidebar_width,
                        height=60,
                    ),
                    bgcolor=cg.get_color(Color.LIGHT),
                )
            ],
            width=cg.sidebar_width,
            height=60,
            vertical_alignment=ft.CrossAxisAlignment.START,
            visible=False,
        )


    @property
    def is_visible(self) -> bool:
        return self.visible


    @is_visible.setter
    def is_visible(self, val: bool) -> None:
        self.visible = val


    def reset(self) -> None:
        self.text_field.value = ""
        self.update()


    def is_date_validated(self) -> bool:
        if not self.text_field.value:
            return True
        try:
            date = datetime.strptime(self.text_field.value, Date_Format.DD_MM_YYYY.value)
            if date.date() >= datetime.today().date():
                return True
            else:
                return False
        except ValueError:
            return False


    def get_deadline(self) -> Union[str, None]:
        return None if len(self.text_field.value) == 0 \
            else datetime.strftime(
                datetime.strptime(self.text_field.value, Date_Format.DD_MM_YYYY.value),
                Date_Format.DD_MM_YYYY.value
            )

