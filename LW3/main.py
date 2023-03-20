import lib_cppgeometry_wrapper as l
import dearpygui_ext.themes as dpg_ext
import dearpygui.dearpygui as dpg
import gui
import random
from typing import Callable

plot_number_amount = 5


def get_text_updater(item_id: int | str, handler: Callable[..., str]):
   """Generate handler which updates text item

   Args:
       item_id (int | str): text's id
       handler (Callable[..., str]): procedure which
       generates content by incoming item data. Function calls
       handler as handler(sender_value, current_text_value)
   """
   def __update(sender: int | str):
      """Generated handler

      Args:
          sender (int | str): Default handler parameter
      """
      val = dpg.get_value(sender)
      out = handler(val, dpg.get_value(item_id))
      dpg.set_value(item_id, out)
   return __update


get_label_labels = "ABCDEFGHIJKLMPRSTUFXYZ"
get_label_used = []
max_points = len(get_label_labels)*10
text_updaters = {}
item_updaters = {}
points = []


def method_jarvis(points):
   return l.convexHull(points, l.ConvexHullMethod.JARVIS)


def method_grehem(points):
   return l.convexHull(points, l.ConvexHullMethod.JARVIS)


method_map = {"Алгоритм Джарвиса": method_jarvis,
              "Алгоритм Грэхема": method_grehem, }
current_method = "Алгоритм Джарвиса"


def generate_label_free():
   global get_label_used
   get_label_used = []


def generate_label():
   global get_label_labels, get_label_used
   start = 0
   for suffix in map(str, range(start-1, 10)):
      if suffix == str(start-1):
         suffix = ""
      for c in get_label_labels:
         label = c + suffix
         if label in get_label_used:
            continue
         get_label_used.append(label)
         return label
   raise RuntimeError("Cannot generate label: maximum values exceeded")


def generate_color():
   return random.choices(range(256), k=3)


def generate_point_round(precision):
   def __round(value):
      return l.Point(round(value["x"], precision), round(value["x"], precision))
   return __round


def generate_point(plot_id, point_callback):
   generate_label_free()

   def __update():
      global points
      points = []
      dpg.delete_item(plot_id, children_only=True)
      for i in range(plot_number_amount):
         count, max_count = (0, 10)
         _round = generate_point_round(2)
         while True:
            p = l.Point.getRandom(0, 3, 2)
            if _round(p) in map(_round, points):
               continue
            if count > max_count:
               break
            count += 1
         points.append(p)
         dpg.add_drag_point(default_value=(
             p["x"], p['y']), label=generate_label(), callback=point_callback, parent=plot_id, color=generate_color())
   return __update


def set_plot_number_amount(sender):
   global plot_number_amount, max_points
   strval = dpg.get_value(sender)

   val = int(strval) if strval != '' else plot_number_amount
   if val > max_points:
      dpg.set_value(sender, plot_number_amount)
   else:
      plot_number_amount = int(val)


def set_current_method(sender, user_data):
   global current_method
   current_method = str(user_data)


def make_hull():
   method_map[current_method]([l.Point(1.0, 1.0)])


def window():
   global ui_text, plot_number_amount

   w = dpg.add_window(label="main window")

   with dpg.menu_bar(parent=w):
      bar_generate = dpg.add_menu_item(label="Сгенерировать точки")

      dpg.add_menu_item(label="Построить выпуклую оболочку",
                        callback=make_hull)

      with dpg.menu(label="Настройки"):
         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Количество точек: ")

            dpg.add_input_text(
                width=120, decimal=True, hint="Значение", callback=set_plot_number_amount)
         dpg.add_text(
             default_value=f"\t\t(по умолчанию: {plot_number_amount})")
         with dpg.group(horizontal=True):
            dpg.add_text(default_value="Метод построения: ")
            dpg.add_listbox(items=list(method_map.keys()),
                            callback=set_current_method)
      # dpg.add_menu_item(label="Помощь")
   text_updaters['point_coords'] = get_text_updater(dpg.add_text(parent=w), lambda val, _: "x: {:.2f},y: {:.2f}".format(
       round(val[0], 2), round(val[1], 2)))
   with dpg.plot(label="График", height=-1, width=-1, parent=w) as plot:
      item_updaters['plot'] = generate_point(
          plot, text_updaters["point_coords"])
      oX = dpg.add_plot_axis(dpg.mvXAxis, label="x")
      oY = dpg.add_plot_axis(dpg.mvYAxis, label="y")
   dpg.set_primary_window(w, True)
   item_updaters["plot"]()

   dpg.set_item_callback(bar_generate, item_updaters["plot"])


def viewport():
   dpg.create_viewport(title='ConvexHull GUI',
                       width=1000, height=600)


def fix_font(id: str | int):
   dpg.add_font_range_hint(dpg.mvFontRangeHint_Cyrillic, parent=id)


def main():
   theme = dpg_ext.create_theme_imgui_light
   font = ["assets/Montserrat-Regular.otf", 25]

   items = [window, viewport]
   gui.create_gui(gui.Parameters(
       items, theme, font[0], fix_font, font[1], True))


if __name__ == "__main__":
   main()
