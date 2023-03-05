import configparser
import os

from settings.enums import Color



class Config:
    def __init__(self):
        self._config_parser = configparser.ConfigParser()

        self._config_parser['colors'] = {
            'dark': '#333333',
            'light': '#FFF8E7',
            'light_2': "#FFF0DF",
            'light_3': '#FFE8D7',
            'red': '#CA3B38',
            'green': '#85A67B',
            'debug': '#A50AFF',
            'debug_2': '#7600BC',
            'debug_3': '#5A008F',
            'error': '#FF3333'
        }

        self._config_parser['font'] = {
            'default': "PWS"
        }

        self._config_parser['window_options'] = {
            'width': '1400',
            'height': '1000'
        }

        self._config_parser['navibar'] = {
            'width': '70',
            'height': '1000'
        }

        self._config_parser['sidebar'] = {
            'width': '300',
            'height': '1000'
        }

        self._config_parser['task_list'] = {
            'closed_ratio': '3',
            'opened_ratio': '27',
            'sum': '30'
        }

        self._config_parser['content_area'] = {
            'width': '1000',
            'height': '1000'
        }



        with open(os.path.join(os.getcwd(), 'settings/configfile.ini'), 'w') as configfile:
            self._config_parser.write(configfile)


    @property
    def window_width(self) -> int:
        return int(self._config_parser['window_options']['width'])

    @property
    def window_height(self) -> int:
        return int(self._config_parser['window_options']['height'])


    @property
    def navibar_width(self) -> int:
        return int(self._config_parser['navibar']['width'])

    @property
    def navibar_height(self) -> int:
        return int(self._config_parser['sidebar']['navibar'])


    @property
    def sidebar_width(self) -> int:
        return int(self._config_parser['sidebar']['width'])

    @property
    def sidebar_height(self) -> int:
        return int(self._config_parser['sidebar']['height'])

    def get_color(self, color: Color) -> str:
        try:
            return self._config_parser['colors'][color.value]
        except Exception as e:
            print(e)
            return self._config_parser['colors']['error']


    def get_task_list_width(self, is_opened: bool) -> int:
        width: int = int(self._config_parser['sidebar']['width'])
        width *= int(self._config_parser['task_list']['opened_ratio']) if is_opened else int(self._config_parser['task_list']['closed_ratio'])
        width //= int(self._config_parser['task_list']['sum'])
        return width

    def font(self, name: str = None) -> str:
        return self._config_parser['font']['default'] if name is None \
            else self._config_parser['font'][name]


cg = Config()


