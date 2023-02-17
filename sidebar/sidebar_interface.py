from settings.config_init import cg
from settings.enums import Color
import flet as ft



class Sidebar(ft.Container):
    def __init__(self, page: ft.Page):
        super().__init__(
            width=cg.sidebar_width,
            height=cg.sidebar_height,
            padding=0,
            bgcolor=cg.get_color(Color.LIGHT),
            left=-cg.sidebar_width,
            animate_position = 200,
        )
        self.page = page

        self.main_col = ft.Column(
            width=self.width,
            height=cg.window_height,
        )

        self.content = self.main_col


    def hide(self, e):
        self.left = -cg.sidebar_width
        self.update()

    def show(self, e):
        self.left = 0
        self.update()

