import flet as ft

from settings.config_init import cg
from settings.enums import Color

from sidebar.instances.todo_dir.task_list.task_list_instance import Task_List_Instance



class On_Going_Task_List(Task_List_Instance):
    def __init__(self, is_opened: bool):
        super().__init__(is_opened)

        self.bgcolor = cg.get_color(Color.LIGHT)
