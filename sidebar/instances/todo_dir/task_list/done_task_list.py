import flet as ft

from settings.config_init import cg
from settings.enums import Color

from sidebar.instances.todo_dir.task_list.task_list_instance import Task_List_Instance



class Done_Task_list(Task_List_Instance):
    def __init__(self, is_opened: bool):
        super().__init__(is_opened)

        self.bgcolor = cg.get_color(Color.GREEN)
        self.left = cg.get_task_list_width(True)

