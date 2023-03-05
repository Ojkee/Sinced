import flet as ft

from settings.config_init import cg
from settings.enums import Color

from sidebar.instances.todo_dir.task_list.task_entity.task_dataclass import Task
from sidebar.instances.todo_dir.task_list.task_entity.task_container import Task_Container_Builder



class Draggable_Task(ft.GestureDetector):
    def __init__(self, task: Task):
        self.content_task = Task_Container_Builder(task)
        super().__init__(
            mouse_cursor=ft.MouseCursor.MOVE,
            content=self.content_task,
            on_tap=self.content_task.info_clicked,
            on_horizontal_drag_update=self.drag,
            on_horizontal_drag_end=self.on_drag_end,
        )
        self.base_left: int = (cg.get_task_list_width(True) - self.content_task.Width) // 4
        self.left = self.base_left
        self.max_drag_delta_x: int = 120

        self.accept_range: int = 16


    def drag(self, e: ft.DragUpdateEvent):
        current_left = self.left + int(e.delta_x)
        if self.base_left - self.max_drag_delta_x <= current_left <= self.base_left + self.max_drag_delta_x:
            self.left = current_left

        self.drag_gradient()
        self.update()


    def drag_gradient(self) -> None:
        proportion = (self.left - self.base_left) / self.max_drag_delta_x
        target_color = Color.GREEN if proportion > 0 else Color.RED
        target_color_str = cg.get_color(target_color)
        curr_color = self.content_task.current_status_color
        lerped_color_str = self.lerp_color(curr_color, target_color_str, abs(proportion))
        self.content_task.set_current_color(lerped_color_str)


    @staticmethod
    def lerp_color(color1: str, color2: str, proportion: float) -> str:
        r1 = int(color1[1:3], 16)
        g1 = int(color1[3:5], 16)
        b1 = int(color1[5:7], 16)

        r2 = int(color2[1:3], 16)
        g2 = int(color2[3:5], 16)
        b2 = int(color2[5:7], 16)
        def re_prop(x: float) -> float:
            intensity: int = 9
            return (pow(intensity, x) - 1) / (intensity - 1)
        def norm_lerp(c1, c2) -> str:
            color_dec = c1 + int((c2 - c1) * re_prop(proportion))
            result = '{:02x}'.format(color_dec)
            return result.upper()
        return f'#{norm_lerp(r1, r2)}{norm_lerp(g1, g2)}{norm_lerp(b1, b2)}'


    def on_drag_end(self, e):
        ejection = self.left - self.base_left
        ejection_range = self.max_drag_delta_x - abs(ejection)
        if ejection_range > self.accept_range:
            self.reset_drag_task()
        elif ejection < 0:
            print("TO DO ADD TO DONE TASK")
        elif ejection > 0:
            print("TO DO DELETE TASK")

        self.update()


    def reset_drag_task(self) -> None:
        self.left = self.base_left
        self.content_task.reset_color()
