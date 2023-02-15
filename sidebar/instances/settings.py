import flet as ft
from sidebar.sidebar_interface import Sidebar


class Settings(Sidebar):
    def __init__(self, page: ft.Page):
        super().__init__(page)


