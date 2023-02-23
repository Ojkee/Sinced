from enum import Enum


class Color(Enum):
    DARK = 'dark'
    LIGHT = 'light'
    LIGHT_2 = 'light_2'
    LIGHT_3 = "light_3"
    RED = 'red'
    GREEN = 'green'
    DEBUG = 'debug'


class Savefile_Path(Enum):
    TASKS_ON_GOING = 'saves/tasks_on_going.json'
    TASKS_DONE = 'saves/tasks_done.json'


class Img_Path(Enum):
    BACK_BUTTON = 'imgs/back_button.png'
    TODO = 'imgs/todo_icon.png'
    TODO_SELECTED = 'imgs/todo_selected.png'
    PEN = 'imgs/pen.png'
    PENCIL = 'imgs/pencil/png'


class Date_Format(Enum):
    DD_MM_YYYY = "%d-%m-%Y"

