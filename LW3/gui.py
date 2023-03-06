import dearpygui.dearpygui as dpg
from dataclasses import dataclass
from typing import Callable

@dataclass
class Parameters:
   """   Parameters structure

   Args:
      theme (str | int): theme to use
      font (str | int): font to use
      windows list[function]: functions with callbacks and window definitions inside
   """
   items: list[Callable[[], None]]
   theme: Callable[[], (str | int)] | None
   font: str | None
   font_callback: Callable[[str | int], None] | None
   font_size: int = 12


def create_gui(info: Parameters, default_viewport: bool = False):
   dpg.create_context()

   if info.theme is not None:
      theme_id = info.theme()
      dpg.bind_theme(theme_id)

   if info.font is not None:
      with dpg.font_registry():
         # first argument ids the path to the .ttf or .otf file
         font = dpg.add_font(info.font, info.font_size)
         if info.font_callback is not None:
            info.font_callback(font)
         dpg.bind_font(font)

   for f in info.items:
      f()

   if default_viewport:
      dpg.create_viewport(title='Default viewport',
                          width=800, height=600)
   dpg.setup_dearpygui()
   dpg.show_viewport()
   dpg.start_dearpygui()
   dpg.destroy_context()
