from enum import Enum


class Color(Enum):
    DARK = 'dark'
    LIGHT = 'light'
    LIGHT_2 = 'light_2'
    LIGHT_3 = "light_3"
    RED = 'red'
    GREEN = 'green'
    DEBUG = 'debug'
    DEBUG_2 = 'debug_2'
    DEBUG_3 = 'debug_3'


class Savefile_Path(Enum):
    TASKS = 'saves/tasks.json'


class Img_Path(Enum):
    BACK_BUTTON = 'imgs/back_button.png'
    TODO = 'imgs/todo_icon.png'
    TODO_SELECTED = 'imgs/todo_selected.png'
    PEN = 'imgs/pen.png'
    PENCIL = 'imgs/pencil/png'


class Date_Format(Enum):
    DD_MM_YYYY = "%d-%m-%Y"


class Task_Status(Enum):
    CANCELED = 'canceled'
    ON_GOING = 'on_going'
    DONE = 'done'
    EXPIRED = 'expired'
    EXTENDED = 'extended'

