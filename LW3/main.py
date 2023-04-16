import lib_cppgeometry_wrapper as l
import dearpygui_ext.themes as dpg_ext
import dearpygui.dearpygui as dpg
import gui
from typing import Callable
from events import EventHandlerPool
from event_handlers import *
import sys

# Default value
plot_number_amount = 5
point_min_val = 0
point_max_val = 3

method_map_keys = [
    "Алгоритм Джарвиса (заворачивания подарка)", "Алгоритм Грэхема"]
method_map = {method_map_keys[0]: l.ConvexHullMethod.JARVIS,
              method_map_keys[1]: l.ConvexHullMethod.GRAHAM, }

# Default value
current_method = method_map_keys[0]

e_pool = {}


def window():
   global ui_text, plot_number_amount

   global button_bar_gen, button_make_hull, \
      input_point_amount, input_method, text, plot, __points, \
       input_min_val, input_max_val, input_min_val
   __points = {}

   w = dpg.add_window()

   with dpg.menu_bar(parent=w):
      button_bar_gen = dpg.add_menu_item(label="Сгенерировать точки")

      button_make_hull = dpg.add_menu_item(
          label="Построить выпуклую оболочку", enabled=False)

      with dpg.menu(label="Настройки"):
         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Количество точек: ")

            input_point_amount = dpg.add_input_int(
                width=120, default_value=plot_number_amount)
            dpg.add_text(
                default_value=f"(по умолчанию: {plot_number_amount})")
         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Метод построения: ")
            input_method = dpg.add_listbox(items=list(method_map.keys()))
         dpg.add_text(default_value="Параметры генерации точек: ")
         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Min: ")
            input_min_val = dpg.add_input_int(
                default_value=0, width=150, user_data="min")
            dpg.add_text(default_value="Max: ")
            input_max_val = dpg.add_input_int(
                default_value=3, width=150, user_data="max")

   text = dpg.add_text(parent=w)
   plot = dpg.add_plot(label="График", height=-1, width=-1, parent=w)
   dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot)
   dpg.set_primary_window(w, True)


def set_callbacks():
   global __points
   # Static UI objects pools
   e_pool["virt_w_text"] = EventHandlerPool(text, False)
   e_pool["stc_btn_point_gen"] = EventHandlerPool(button_bar_gen)
   e_pool["stc_settings_point_amount"] = EventHandlerPool(input_point_amount)
   e_pool["stc_settings_method"] = EventHandlerPool(input_method)
   e_pool["stc_settings_min_val"] = EventHandlerPool(input_min_val)
   e_pool["stc_settings_max_val"] = EventHandlerPool(input_max_val)
   e_pool["stc_btn_make_hull"] = EventHandlerPool(button_make_hull)
   # Dynamic point pool
   e_pool["dyn_point"] = EventHandlerPool(0, False)

   # Pool configuration
   e_pool["dyn_point"] += e_pool["virt_w_text"].get_bind_handler(
       0, PointHandlers.process_to_text, use_sender=True)
   e_pool["dyn_point"] += PointHandlers.update_lines()
   e_pool["dyn_point"] += PointHandlers.update_points()
   e_pool["dyn_point"] += PointHandlers.print_points()

   e_pool["stc_btn_point_gen"] += ButtonHandlers.generate_point(
       plot, e_pool["dyn_point"], [button_make_hull])
   e_pool["stc_settings_point_amount"] += TextHandlers.set_plot_number_amount
   e_pool["stc_settings_method"] += ListBoxHandlers.set_current_method
   e_pool["stc_btn_make_hull"] += ButtonHandlers.make_hull_by_points
   e_pool["stc_settings_min_val"] += TextHandlers.point_set_min_val
   e_pool["stc_settings_max_val"] += TextHandlers.point_set_max_val


def viewport():
   dpg.create_viewport(title='ConvexHull GUI',
                       width=1000, height=600)


def fix_font(id: str | int):
   dpg.add_font_range_hint(dpg.mvFontRangeHint_Cyrillic, parent=id)


def main():
   theme = dpg_ext.create_theme_imgui_light
   font = [sys.path[0] + "/assets/Montserrat-Regular.otf", 25]

   items = [window, viewport, set_callbacks]
   gui.create_gui(gui.Parameters(
       items, theme, font[0], fix_font, font[1], True))


if __name__ == "__main__":
   main()
