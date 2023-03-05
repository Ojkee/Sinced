import flet as ft
from datetime import datetime
from typing import Union

from settings.config_init import cg
from settings.enums import Color, Date_Format, Task_Status

from sidebar.instances.todo_dir.task_list.task_entity.task_dataclass import Task


class Task_Container_Builder(ft.Container):
    def __init__(
            self,
            task: Task
    ):
        self._width: int = cg.get_task_list_width(is_opened=True) * 17 // 20
        self.task_data = task
        self.text_size = 16
        self.ratio: tuple[int, int] = (7, 3)


        self.base_color = Color.LIGHT_2
        self.clicked_color = Color.LIGHT_3
        self.font_color = Color.DARK

        self.current_base_color_value = cg.get_color(self.base_color)
        self.current_clicked_color_value = cg.get_color(self.clicked_color)


        self.text_info = self.build_task_info()
        self.modify_deadline_text_field = self.build_modify_deadline_text_field()
        self.modify_task_field = self.build_modify_task_field()


        self.main_task_row = ft.Column(
            controls=[
                self.text_info,
                self.modify_task_field
            ],
        )

        super().__init__(
            content=self.main_task_row,
            bgcolor=self.current_base_color_value,
            border_radius=20,
            padding=10,
        )

    def __repr__(self) -> str:
        return self.task_data.__repr__()


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


    @property
    def Width(self) -> int:
        return self._width

    @Width.setter
    def Width(self, val: int) -> None:
        self._width = val


    @property
    def current_status_color(self) -> str:
        return cg.get_color(self.clicked_color) if self.modify_task_field.visible else cg.get_color(self.base_color)

    def set_current_color(self, color: str) -> None:
        if self.modify_task_field.visible:
            self.current_clicked_color_value = color
            self.bgcolor = self.current_clicked_color_value
        else:
            self.current_base_color_value = color
            self.bgcolor = self.current_base_color_value
        self.update()

    def reset_color(self) -> None:
        self.current_base_color_value = cg.get_color(self.base_color)
        self.current_clicked_color_value = cg.get_color(self.clicked_color)
        self.bgcolor = cg.get_color(self.clicked_color) if self.modify_task_field.visible else cg.get_color(self.base_color)
        self.update()


    def build_task_info(self) -> ft.Container:
        remaining = "" if self.task_data.deadline is None else self.get_remaining_time_to_deadline(self.task_data.deadline)
        main_row = ft.Row(
            controls=[
                ft.Container(
                    content=ft.Text(
                        value=self.task_data.text,
                        color=cg.get_color(self.font_color),
                        font_family=cg.font(),
                        weight=ft.FontWeight.W_700,
                        size=self.text_size,
                    ),
                    width=self._width * self.ratio[0] // sum(self.ratio),
                    alignment=ft.alignment.center_left,
                ),
                ft.Container(
                    content=ft.Text(
                        value=remaining,
                        color=cg.get_color(self.font_color),
                        font_family=cg.font(),
                        weight=ft.FontWeight.W_700,
                        size=self.text_size // 6 * 5,
                        ),
                    width=self._width * self.ratio[1] // sum(self.ratio),
                    alignment=ft.alignment.center_right,
                ),
            ],
            alignment=ft.MainAxisAlignment.SPACE_BETWEEN,
            width=self._width,
            spacing=10,
        )
        return ft.Container(
            content=main_row,
        )


    def build_modify_deadline_text_field(self) -> ft.TextField:
        text_field = ft.TextField(
            value=self.task_data.get_deadline_str(),
            text_style=ft.TextStyle(
                color=cg.get_color(self.font_color),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
            ),
            label="modify deadline",
            label_style=ft.TextStyle(
                color=cg.get_color(self.font_color),
                font_family=cg.font(),
                weight=ft.FontWeight.BOLD,
            ),
            hint_text="dd-mm-yyyy",
            hint_style=ft.TextStyle(
                color=cg.get_color(self.font_color),
                font_family=cg.font(),
                italic=True,
            ),
            border=ft.InputBorder.NONE,
            border_color=cg.get_color(self.font_color),
            cursor_color=cg.get_color(self.font_color),
            content_padding=10,
            on_submit=self.modify_task_field_on_submit,
        )
        return text_field


    def build_modify_task_field(self) -> ft.Row:
        main_row = ft.Row(
            controls=[
                ft.Container(
                    content=self.modify_deadline_text_field,
                    width=self._width * self.ratio[0] // sum(self.ratio),
                ),
                ft.Container(
                    ft.Image(
                        src="imgs/pen.png",
                        fit=ft.ImageFit.SCALE_DOWN,
                        color=cg.get_color(self.font_color),
                    ),
                    width=self._width * self.ratio[1] // sum(self.ratio),
                    height=40,
                    on_click=self.modify_task_field_on_submit,
                )
            ],
            alignment=ft.MainAxisAlignment.SPACE_BETWEEN,
            visible=False,
            width=self._width,
        )
        return main_row



    @staticmethod
    def get_remaining_time_to_deadline(date_text: str | None) -> str | None:
        if date_text is None:
            return None
        now = datetime.now()
        delta = datetime.strptime(date_text, Date_Format.DD_MM_YYYY.value) - now
        return f"{delta.days + 1}\ndays" if delta.days + 1 != 1 else "1\nday"


    def info_clicked(self, e) -> None:
        if not self.modify_task_field.visible:
            self.expand_info(e)
            self.bgcolor = self.current_clicked_color_value
        else:
            self.hide_info(e)
            self.bgcolor = self.current_base_color_value
        self.modify_task_field.update()
        self.update()


    def modify_task_field_on_submit(self, e) -> None:
        if self.is_date_validated():
            self.task_data.deadline = self.get_deadline_from_text_field()
            self.update_text_info()
            self.modify_task_field.visible = False
        self.update()


    def update_text_info(self) -> None:
        self.text_info.content.controls[1].content.value = self.get_remaining_time_to_deadline(
            self.get_deadline_from_text_field()
        )

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



