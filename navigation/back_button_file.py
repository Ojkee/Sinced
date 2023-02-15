import flet as ft
from math import pi

from settings.config_init import cg
from settings.enums import Color, Img_Path




class Back_Button(ft.Container):
    def __init__(self, on_click):
        self.sidebar_containter_onclick = on_click
        self.to_left: bool = True
        super().__init__(
            content=ft.Row(
                controls=[
                    ft.Image(
                        src=Img_Path.BACK_BUTTON.value,
                        fit=ft.ImageFit.FILL,
                        width=40,
                        height=40,
                        color=cg.get_color(Color.LIGHT)
                    )
                ],
                alignment=ft.MainAxisAlignment.CENTER,
                vertical_alignment=ft.CrossAxisAlignment.CENTER,
                width=90,
                height=90,
            ),
            rotate=ft.transform.Rotate(0, alignment=ft.alignment.center),
            animate_rotation=ft.animation.Animation(100, ft.AnimationCurve.BOUNCE_OUT),
            bgcolor=cg.get_color(Color.DARK),
            on_click=self.backbutton_click,
        )


    def backbutton_click(self, e):
        self.sidebar_containter_onclick(e)
        # self.rotate.angle += pi
        # self.update()




