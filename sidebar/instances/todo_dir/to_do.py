import flet as ft

from settings.config_init import cg
from settings.enums import Color

from sidebar.sidebar_interface import Sidebar
from sidebar.instances.todo_dir.to_do_text_field import To_Do_Text_Field
from sidebar.instances.todo_dir.text_field_date_row import Text_Field_Date_Row
from sidebar.instances.todo_dir.task_list.task_list_manager import Task_List_Manager


class To_Do(Sidebar):
    def __init__(self, page: ft.Page):
        super().__init__(page)
        self.title_row = ft.Row(
            controls=[
                ft.Text(
                    value="To do",
                    color=cg.get_color(Color.DARK),
                    style=ft.TextThemeStyle.HEADLINE_LARGE,
                    weight=ft.FontWeight.BOLD,
                    font_family=cg.font(),
                )
            ],
            spacing=0,
            alignment=ft.MainAxisAlignment.CENTER,
            width=cg.sidebar_width,
            height=60,
        )

        self.text_field = To_Do_Text_Field(self.field_text_on_change, self.add_new_task)

        self.submit_button = ft.Container(
            ft.Image(
                src="imgs/pencil.png",
                fit=ft.ImageFit.SCALE_DOWN,
                color=cg.get_color(Color.DARK),
            ),
            width=60,
            height=40,
            on_click=self.add_new_task,
        )

        self.field_text_row = ft.Row(
            controls=[
                ft.Row(
                    controls=[
                        self.text_field,
                        self.submit_button,
                    ],
                    width=cg.sidebar_width,
                    height=60,
                ),
            ],
            vertical_alignment=ft.CrossAxisAlignment.START,
            width=cg.sidebar_width,
        )
        self.new_task_option = Text_Field_Date_Row(self.add_new_task)
        self.tasks_list = Task_List_Manager()

        self.main_col.controls.extend([
            self.title_row,
            self.field_text_row,
            self.new_task_option,
            self.tasks_list
        ])


    def add_new_task(self, e):
        if self.new_task_option.is_date_validated() and not self.text_field.is_empty():
            self.tasks_list.add_task(
                self.text_field.get_normalized_task_text(),
                self.new_task_option.get_deadline()
            )
            self.new_task_option.reset()
            self.new_task_option.visible = False

            self.tasks_list.update()
            self.new_task_option.update()
            self.text_field.reset()


    def field_text_on_change(self, e):
        if not self.text_field.is_empty():
            self.new_task_option.is_visible = True
        else:
            self.new_task_option.is_visible = False
        self.new_task_option.update()


