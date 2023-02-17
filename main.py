import flet as ft

from settings.config_init import cg
from settings.enums import Color

from sidebar.sidebar_container import Sidebar_Container
from navigation.navigation_bar import Navigation_Bar


class MainUI(ft.UserControl):
    def __init__(self, page, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._active_view = None
        self.page = page
        self.main = None
        self.main_row = None
        self.sidebar_container = Sidebar_Container(page)
        self.navigation_bar = Navigation_Bar(page, self.sidebar_container)

    def main_container(self):
        self.main = ft.Container(
            padding=0,
            width=cg.window_width,
            height=cg.window_height,
            bgcolor=cg.get_color(Color.DARK)
        )

        self.main_row = ft.Row(
            height=cg.window_height,
            spacing=0,
        )

        self.main_row.controls.append(
            ft.Container(
                self.navigation_bar.rail,
                margin=0,
            )
        )

        self._active_view: ft.Control = ft.Column(
            controls=[
                ft.Text(
                    "Active View",
                    font_family=cg.font()
                )
            ],
            alignment=ft.MainAxisAlignment.CENTER,
            horizontal_alignment=ft.CrossAxisAlignment.CENTER,
        )

        self.main_row.controls.extend([self.sidebar_container.main_stack, self._active_view])
        self.main.content = self.main_row
        return self.main


    def build(self) -> ft.Column:
        return ft.Column(
            controls=[
                self.main_container(),
            ]
        )



if __name__ == '__main__':
    def main(page: ft.Page) -> None:
        page.title = "Sinced"
        page.padding = 0
        page.spacing = 0
        page.window_width = cg.window_width
        page.window_height = cg.window_height
        page.bgcolor = cg.get_color(Color.DARK)
        page.fonts = {
            "PWS": "fonts/PWSimpleHandwriting.ttf",
        }
        page.theme = ft.Theme(font_family=cg.font())


        app = MainUI(page)
        page.add(app)
        page.update()
    ft.app(target=main, assets_dir="assets")
