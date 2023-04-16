import random
import dearpygui.dearpygui as dpg
import lib_cppgeometry_wrapper as l
import events
from main import current_method, method_map, plot_number_amount,\
    point_min_val, point_max_val

_line_series_id = 0
_get_label_labels = "ABCDEFGHIJKLMPRSTUFXYZ"
_points = {}
_max_points = len(_get_label_labels)*10
_axis_Oy_id = 0
_lines = {}


class ListBoxHandlers:
   @staticmethod
   def set_current_method(sender, user_data, _0=None):
      global current_method
      current_method = str(user_data)


class TextHandlers:
   @staticmethod
   def set_plot_number_amount(sender, _0=None, _1=None):
      global plot_number_amount, _max_points
      strval = dpg.get_value(sender)

      val = int(strval) if strval != '' else plot_number_amount
      if val > _max_points:
         dpg.set_value(sender, plot_number_amount)
      else:
         plot_number_amount = int(val)

   @staticmethod
   def point_set_min_val(sender, _0=None, _1=None):
      global point_max_val, point_min_val
      val = dpg.get_value(sender)

      if val < point_max_val:
         point_min_val = val
      else:
         dpg.set_value(sender, point_min_val)

   @staticmethod
   def point_set_max_val(sender, _0=None, _1=None):
      global point_max_val, point_min_val
      val = dpg.get_value(sender)

      if val > point_min_val:
         point_max_val = val
      else:
         dpg.set_value(sender, point_max_val)


class ButtonHandlers:
   @staticmethod
   def make_hull_by_points(sender: int | str, _0=None, _1=None):
      global _line_series_id, _points, _axis_Oy_id, _lines
      raw_points = list(_points.values())

      convex_hull = l.Polygon.convexHull(l.VectorPoint(
          raw_points), method_map[current_method])
      x, y = get_series_from_point_list(convex_hull)  # type: ignore
      for p in convex_hull:
         if p in _points.values():
            label_index = list(_points.values()).index(p)
            label = list(_points.keys())[label_index]
            _lines[label] = p
      dpg.delete_item(_line_series_id)
      _line_series_id = dpg.add_line_series(x, y,
                                            label='Многоугольник', parent=_axis_Oy_id)

   @staticmethod
   def generate_point(plot_id: int | str, point_ep: events.EventHandlerPool, disabled_items: list[int | str]):

      _round_func = round_point(2)

      def __update_func(sender: int | str, _0=None, _1=None):
         global _axis_Oy_id, _points, _line_series_id, point_min_val, point_max_val
         _points = {}
         dpg.delete_item(plot_id, children_only=True)
         _line_series_id = 0

         dpg.add_plot_axis(dpg.mvXAxis, label='x', parent=plot_id)
         _axis_Oy_id = dpg.add_plot_axis(
             dpg.mvYAxis, label='y', parent=plot_id)

         for i in disabled_items:
            dpg.enable_item(i)

         for _ in range(plot_number_amount):
            count, max_count = (0, 10)
            while True:
               p = l.Point.getRandom(point_min_val, point_max_val, 2)
               if count > max_count:
                  break
               if _round_func(p) not in list(map(_round_func, list(_points.values()))):
                  break
               count += 1
            label = generate_label(_points)
            _points[label] = p
            dpg.add_drag_point(default_value=(
                p['x'], p['y']), label=label, callback=point_ep, parent=plot_id, color=generate_color())
      return __update_func


class PointHandlers:
   @staticmethod
   def print_points():
      def __update_func(sender: int | str, _0=None, _1=None):
         global _points
         print("Points")
         for i in _points:
            print(str(_points[i]), end="\n")
         print("---")
      return __update_func

   @staticmethod
   def update_lines():
      def __update_func(sender: int | str, _0=None, _1=None):
         global _line_series_id, _axis_Oy_id, _lines
         label = dpg.get_item_label(sender)
         data = dpg.get_value(sender)
         new_x, new_y = data[0], data[1]
         _lines[label] = l.Point(new_x, new_y)
         x, y = get_series_from_point_dict(points=_lines)  # type: ignore
         id = str(_line_series_id)
         if len(id) > 0 and id != "0":
            dpg.delete_item(_line_series_id)
            _line_series_id = dpg.add_line_series(x, y,
                                                  label='Многоугольник',
                                                  parent=_axis_Oy_id)
      return __update_func

   @staticmethod
   def update_points():
      def __update_func(sender: int | str, _0=None, _1=None):
         global _line_series_id, _axis_Oy_id, _points
         label = dpg.get_item_label(sender)
         data = dpg.get_value(sender)
         new_x, new_y = data[0], data[1]
         _points[label] = l.Point(new_x, new_y)
      return __update_func

   @staticmethod
   def process_to_text(_, point_val): return "x: {:.2f},y: {:.2f}".format(
       round(point_val[0], 2), round(point_val[1], 2))


def generate_label(points: dict[str, l.Point]):
   global _get_label_labels
   start = 0
   for suffix in map(str, range(start-1, 10)):
      if suffix == str(start-1):
         suffix = ""
      for c in _get_label_labels:
         label = c + suffix
         if label in list(points.keys()):
            continue
         points[label] = None
         return label
   raise RuntimeError("Cannot generate label: maximum values exceeded")


def generate_color():
   return random.choices(range(256), k=3)


def round_point(precision):
   def __round_func(value):
      return l.Point(round(value['x'], precision), round(value['x'], precision))
   return __round_func


def get_series_from_point_list(points: list[l.Point]):
   series_x, series_y = [], []
   for i in list(points):
      series_x.append(i['x'])
      series_y.append(i['y'])
   series_x.append(series_x[0])
   series_y.append(series_y[0])
   return series_x, series_y


def get_series_from_point_dict(points: dict[str, l.Point]):
   return get_series_from_point_list(list(points.values()))
