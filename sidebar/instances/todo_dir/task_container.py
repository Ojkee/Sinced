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


    def get_deadline_str(self) -> str:
        return "" if self.deadline is None else f"{self.deadline}"


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
        self.ratio: tuple[int, int] = (7, 3)

        self.text_info = self.build_task_info()
        self.modify_deadline_text_field = ft.TextField(
            value=self.task_data.get_deadline_str(),
            text_style=ft.TextStyle(
                color=cg.get_color(Color.DARK),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
            ),
            label="modify deadline",
            label_style=ft.TextStyle(
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
            border=ft.InputBorder.NONE,
            border_color=cg.get_color(Color.DARK),
            cursor_color=cg.get_color(Color.DARK),
            content_padding=10,
            on_submit=self.modify_task_field_on_submit,
        )
        self.modify_task_field = self.build_modify_task_field()


        self.main_task_row = ft.Column(
            controls=[
                self.text_info,
                self.modify_task_field
            ],
        )

        super().__init__(
            content=self.main_task_row,
            bgcolor=cg.get_color(Color.LIGHT_2),
            border_radius=20,
            padding=10
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
                    width=cg.sidebar_width * self.ratio[0] // sum(self.ratio),
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
                    width=cg.sidebar_width * self.ratio[1] // sum(self.ratio),
                    horizontal_alignment=ft.CrossAxisAlignment.CENTER,
                    alignment=ft.MainAxisAlignment.CENTER
                ),
            ],
            width=cg.sidebar_width,
            spacing=10,
        )
        return ft.Container(
            content=main_row,
            on_click=self.info_clicked
        )


    @staticmethod
    def get_remaining_time_to_deadline(date_text: str | None) -> str | None:
        if date_text is None:
            return None
        now = datetime.now()
        delta = datetime.strptime(date_text, Date_Format.DD_MM_YYYY.value) - now
        return f"{delta.days}\ndays"


    def build_modify_task_field(self) -> ft.Row:
        main_row = ft.Row(
            controls=[
                ft.Container(
                    content=self.modify_deadline_text_field,
                    width=cg.sidebar_width * self.ratio[0] // sum(self.ratio),
                )
            ],
            visible=False
        )
        return main_row


    def info_clicked(self, e) -> None:
        if not self.modify_task_field.visible:
            self.expand_info(e)
            self.bgcolor = cg.get_color(Color.LIGHT_3)
        else:
            self.hide_info(e)
            self.bgcolor = cg.get_color(Color.LIGHT_2)
        self.modify_task_field.update()
        self.update()


    def modify_task_field_on_submit(self, e) -> None:
        if self.is_date_validated():
            self.task_data.deadline = self.get_deadline_from_text_field()
        self.update_text_info()
        self.modify_task_field.visible = False
        self.update()


    def update_text_info(self) -> None:
        self.text_info.content.controls[1].controls[0].value = self.get_remaining_time_to_deadline(self.get_deadline_from_text_field())


    def expand_info(self, e) -> None:
        self.modify_task_field.visible = True

    def hide_info(self, e) -> None:
        self.modify_task_field.visible = False



    def is_date_validated(self) -> bool:
        if not self.modify_deadline_text_field.value:
            return True
        try:
            date = datetime.strptime(self.modify_deadline_text_field.value, Date_Format.DD_MM_YYYY.value)
            if date.date() >= datetime.today().date():
                return True
            else:
                return False
        except ValueError:
            return False


    def get_deadline_from_text_field(self) -> Union[str, None]:
        return None if len(self.modify_deadline_text_field.value) == 0 \
            else datetime.strftime(
                datetime.strptime(self.modify_deadline_text_field.value, Date_Format.DD_MM_YYYY.value),
                Date_Format.DD_MM_YYYY.value
            )



