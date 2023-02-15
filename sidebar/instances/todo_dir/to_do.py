import flet as ft

from sidebar.instances.todo_dir.task import Task
from settings.config_init import cg
from settings.enums import Color
from sidebar.sidebar_interface import Sidebar



class To_Do(Sidebar):
    def __init__(self, page: ft.Page):
        super().__init__(page)
        self.title_row = ft.Row(
            controls=[
                ft.Text(
                    value="to do",
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

        self.text_field = ft.TextField(
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
            on_change=self.field_text_on_change
        )


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


        self.new_task_option = ft.Row(
            controls=[
                ft.Container(
                    content=ft.Row(
                        width=cg.sidebar_width,
                        height=60,
                    ),
                    bgcolor=cg.get_color(Color.DEBUG),
                )
            ],
            width=cg.sidebar_width,
            height=60,
            vertical_alignment=ft.CrossAxisAlignment.START,
            visible=False,
        )


        self.tasks_list_col = ft.Column(
            controls=[

            ],
            width=cg.sidebar_width,
            scroll=ft.ScrollMode.ALWAYS
        )


        self.main_col.controls.extend([
            self.title_row,
            self.field_text_row,
            self.new_task_option,
            self.tasks_list_col
        ])


    def add_new_task(self, e):
        if self.text_field.value != "" and self.text_field.value.strip():
            self.tasks_list_col.controls.append(
                Task(self.text_field.value.strip()),
            )
            self.tasks_list_col.update()

        self.new_task_option.visible = False
        self.new_task_option.update()
        self.text_field.value = ""
        self.text_field.update()


    def field_text_on_change(self, e):
        if self.text_field.value != "" and self.text_field.value.strip():
            self.new_task_option.visible = True
        else:
            self.new_task_option.visible = False
        self.new_task_option.update()


