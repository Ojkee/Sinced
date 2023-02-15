import flet as ft
from settings.config_init import cg
from sidebar.sidebar_interface import Sidebar


class Roadmap(Sidebar):
    def __init__(self, page: ft.Page):
        super().__init__(page)


