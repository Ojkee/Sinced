import flet as ft

from settings.config_init import cg
from settings.enums import Color, Img_Path
from navigation.back_button_file import Back_Button
from sidebar.sidebar_container import Sidebar_Container



class Navigation_Bar:
    def __init__(self, page: ft.Page, sidebar_container: Sidebar_Container):
        self.page = page
        self.sidebar_container = sidebar_container
        self.rail = ft.NavigationRail(
            selected_index=None,
            label_type=ft.NavigationRailLabelType.ALL,
            width=cg.navibar_width,
            min_extended_width=400,
            height=cg.window_height,
            bgcolor=cg.get_color(Color.DARK),
            leading=Back_Button(self.back_button_on_click),
            group_alignment=-0.9,
            destinations=[
                ft.NavigationRailDestination(
                    icon_content=ft.Container(
                        content=ft.Image(
                            src=Img_Path.TODO.value,
                            fit=ft.ImageFit.FILL,
                            color=cg.get_color(Color.LIGHT)
                        ),
                    ),
                    selected_icon_content=ft.Container(
                        content=ft.Image(
                            src=Img_Path.TODO_SELECTED.value,
                            fit=ft.ImageFit.FILL,
                            color=cg.get_color(Color.LIGHT)
                        ),
                    ),

                ),
                ft.NavigationRailDestination(
                    icon_content=ft.Icon(ft.icons.BOOKMARK_BORDER),
                    selected_icon_content=ft.Icon(ft.icons.BOOKMARK),
                ),
                ft.NavigationRailDestination(
                    icon=ft.icons.SETTINGS_OUTLINED,
                    selected_icon_content=ft.Icon(ft.icons.SETTINGS),

                ),

            ],
            on_change=lambda e: self.sidebar_container.change_sidebar(e, e.control.selected_index),
        )


    def back_button_on_click(self, e) -> None:
        self.sidebar_container.hide(e)
        self.rail.selected_index = None
        self.rail.update()



