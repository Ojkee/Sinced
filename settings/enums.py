from enum import Enum


class Color(Enum):
    DARK = 'dark'
    LIGHT = 'light'
    LIGHT_2 = 'light_2'
    LIGHT_3 = "light_3"
    DEBUG = 'debug'


class Save_File(Enum):
    TASKS = 'tasks.tk'


class Img_Path(Enum):
    BACK_BUTTON = 'imgs/back_button.png'
    TODO = 'imgs/todo_icon.png'
    TODO_SELECTED = 'imgs/todo_selected.png'
    PEN = 'imgs/pen.png'
    PENCIL = 'imgs/pencil/png'


class Date_Format(Enum):
    DD_MM_YYYY = "%d-%m-%Y"

