from typing import Union
import flet as ft

from settings.config_init import cg
from sidebar.instances.todo_dir.to_do import To_Do
from sidebar.instances.roadmap import Roadmap
from sidebar.instances.settings import Settings


class Sidebar_Container:
    def __init__(self, page: ft.Page):
        self.page = page
        self._width: int = cg.sidebar_width
        self._height: int = cg.sidebar_height
        self.main_stack = ft.Stack([
                To_Do(page),
                Roadmap(page),
                Settings(page),
            ],
            width=self._width,
            height=self._height,
        )
        self.last_visible_sidebar: Union[int, None] = None


    def hide(self, e) -> None:
        self.hide_sidebar(e)
        self.main_stack.update()


    def show(self, e) -> None:
        self.main_stack.update()


    def change_sidebar(self, e, index: int) -> None:
        if self.last_visible_sidebar is not None:
            self.hide_sidebar(e)
        self.last_visible_sidebar = index
        self.show_sidebar(e, index)


    def show_sidebar(self, e, index: int) -> None:
        self.show(e)
        self.main_stack.controls[index].show(e)


    def hide_sidebar(self, e) -> None:
        if self.last_visible_sidebar is not None:
            self.main_stack.controls[self.last_visible_sidebar].hide(e)






