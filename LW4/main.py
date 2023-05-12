import lib_cppgeometry_wrapper as l
import dearpygui_ext.themes as dpg_ext
import dearpygui.dearpygui as dpg
import sys
from vars import *
from pathlib import Path
import importlib
from event_handlers import *

bind_pool = {}
"""Array of EventPoolHandler's for binding only
"""
dyn_pool = {}
"""Array of EventPoolHandler's for dynamically generated
   UI elements
"""
s_pool = {}
"""Array of EventPoolHandler's for static UI elements
"""


def import_parents(level=1):
   global __package__
   file = Path(__file__).resolve()
   parent, top = file.parent, file.parents[level]

   sys.path.append(str(top))
   try:
      sys.path.remove(str(parent))
   except ValueError:  # already removed
      pass

   __package__ = '.'.join(parent.parts[len(top.parts):])
   importlib.import_module(__package__)  # won't be needed after that


if __name__ == '__main__' and __package__ is None or __package__ == '':
   import_parents(level=2)
   from ..lib.gui import themes as t
   from ..lib.gui import gui
   from ..lib.gui.events import EventHandlerPool


def window():
   global ui_text, plot_number_amount, listbox_id

   global button_bar_gen, button_clip, \
       input_point_amount, text, plot, \
       input_min_val, input_max_val, input_min_valm, input_theme

   w = dpg.add_window()

   with dpg.menu_bar(parent=w):
      button_bar_gen = dpg.add_menu_item(label="Сгенерировать данные")

      button_clip = dpg.add_menu_item(
          label="Обрезать", enabled=False)

      with dpg.menu(label="Настройки"):
         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Количество точек: ")
            input_point_amount = dpg.add_input_int(
                width=120, default_value=plot_number_amount)
            dpg.add_text(
                default_value=f"(по умолчанию: {plot_number_amount})")

         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Метод обрезания: ")
            listbox_id = dpg.add_listbox(
                items=list(method_map.keys()))
         dpg.add_text(default_value="Параметры генерации точек: ")
         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Min: ")
            input_min_val = dpg.add_input_int(
                default_value=0, width=150, user_data="min")
            dpg.add_text(default_value="Max: ")
            input_max_val = dpg.add_input_int(
                default_value=3, width=150, user_data="max")
         dpg.add_text(default_value="Тема: ")
         input_theme = dpg.add_listbox(items=list(themes.keys()))

   text = dpg.add_text(parent=w)
   plot = dpg.add_plot(label="График", height=-1, width=-1, parent=w)
   dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot)
   dpg.set_primary_window(w, True)


def set_callbacks():
   # Static UI objects pools
   bind_pool["w_text"] = EventHandlerPool(text, False)
   s_pool["btn_point_gen"] = EventHandlerPool(button_bar_gen)
   s_pool["settings_point_amount"] = EventHandlerPool(input_point_amount)
   s_pool["settings_method"] = EventHandlerPool(listbox_id)
   s_pool["settings_min_val"] = EventHandlerPool(input_min_val)
   s_pool["settings_max_val"] = EventHandlerPool(input_max_val)
   s_pool["settings_theme"] = EventHandlerPool(input_theme)
   s_pool["btn_clip"] = EventHandlerPool(button_clip)
   # Dynamic point pool
   dyn_pool["frame_point"] = EventHandlerPool(0, False)

   # Pool configuration
   if debug:
      dyn_pool["frame_point"] += bind_pool["w_text"].get_bind_handler(
          0, PointHandlers.process_to_text, use_sender=True)
      dyn_pool["frame_point"] += PointHandlers.print_points()

   s_pool["settings_point_amount"] += TextHandlers.set_plot_number_amount(
       listbox_id)
   dyn_pool["frame_point"] += PointHandlers.update_frame(
       plot, dyn_pool["frame_point"])

   s_pool["btn_point_gen"] += ButtonHandlers.generate_frame(
       plot, dyn_pool["frame_point"])
   s_pool["btn_point_gen"] += ButtonHandlers.generate_clip_line(plot)
   s_pool["btn_point_gen"] += ButtonHandlers.enable_items([button_clip])
   s_pool["settings_method"] += ListBoxHandlers.set_current_method
   notify_text = bind_pool["w_text"].bind_set_procedure(
       TextHandlers.notify)
   s_pool["btn_clip"] += ButtonHandlers.clip_line(
       notify_text, plot)
   s_pool["settings_min_val"] += TextHandlers.point_set_min_val
   s_pool["settings_max_val"] += TextHandlers.point_set_max_val
   s_pool["settings_theme"] += ListBoxHandlers.set_theme


def viewport():
   dpg.create_viewport(title='ConvexHull GUI',
                       width=1000, height=600)


def fix_font(id: str | int):
   dpg.add_font_range_hint(dpg.mvFontRangeHint_Cyrillic, parent=id)


def main():
   themes["Светлая"] = t.create_theme_white_default
   theme = list(themes.values())[1]
   font = ["../assets/Montserrat-Regular.otf", 25]

   items = [window, viewport, set_callbacks]
   gui.create_gui(gui.Parameters(
       items, theme, font[0], fix_font, font[1], debug))


if __name__ == "__main__":
   main()
