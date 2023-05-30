import random
import dearpygui.dearpygui as dpg
import lib_cppgeometry_wrapper as l
from vars import *
from typing import Callable
import dataclasses
from dataclasses import dataclass

# TODO: round point coordinates to 2 digits (need use dpg.set_value)

labels_alphabet = "ABCDEFGHIJKLMPRSTUFXYZ"
_max_points = 4
_min_points = 3
_axis_Oy_id = 0
default_plot_na = plot_number_amount


@dataclass
class PlotPoint:
   id: int | str = 0
   point: l.Point = dataclasses.field(default_factory=l.Point(0, 0))

   def __init__(self, id: int | str = 0, point: l.Point = l.Point(0, 0)) -> None:
      self.id = id
      self.point = point


_plot_result_series_id = 0
_plot_points = {"": PlotPoint()}
"""Dictionary with structure {"label": PlotPoint}
"""


class ListBoxHandlers:
   @staticmethod
   def set_current_method(sender, user_data, _0=None):
      global current_method
      current_method = str(user_data)

   @staticmethod
   def set_theme(sender, user_data, _0=None):
      global themes
      theme = str(user_data)
      dpg.bind_theme(themes[theme]())


class TextHandlers:
   @staticmethod
   def set_plot_number_amount(make_curve_button_id: int | str):
      def __update_func(sender: int | str, _0=None, _1=None):
         strval = dpg.get_value(sender)
         global plot_number_amount, _max_points

         val = int(strval) if strval != '' else plot_number_amount
         if val > _max_points or val < _min_points:
            dpg.set_value(sender, plot_number_amount)
         else:
            if plot_number_amount != val:
               dpg.disable_item(make_curve_button_id)
            plot_number_amount = val
      return __update_func

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

   @staticmethod
   def notify(msg: str):
      if debug:
         print("DEBUG:", msg)
      return msg


class ButtonHandlers:
   @staticmethod
   def make_curve(plot_id: int | str):
      def __update_func(sender: int | str, _0=None, _1=None):
         global _plot_result_series_id, _plot_points
         points = [i.point for i in list(_plot_points.values())]
         t = tuple(points)
         out = None
         if len(t) == 3:
            out = l.Curve.makeBezierCurveQuadratic(*t)
         elif len(t) == 4:
            out = l.Curve.makeBezierCurveCubic(*t)
         else:
            raise RuntimeError("Amount of points =", len(t), "is invalid.")
         out_x, out_y = get_series_from_list(out)
         dpg_try_delete_item(_plot_result_series_id)
         _plot_result_series_id = dpg.add_line_series(out_x, out_y,  # type: ignore
                                                      label='Кривая Безье', parent=_axis_Oy_id)
      return __update_func

   @staticmethod
   def generate_points(plot_id: int | str, point_callback):
      _round_func = round_point(2)
      _point_size = 2

      def __update_func(sender: int | str, _0=None, _1=None):
         global _plot_result_series_id, _plot_points
         global point_min_val, point_max_val

         dpg_init_oY_axis(plot_id)
         delete_series()
         _plot_points = {}
         # area = l.Point.pp((1,2))

         fractal = l.Fractals.geometricFractal(
             l.Point(0, 0), 1920, 1080, -10, 10, -10, 10, l.GeometricFractalType.KOCH_SNOWFLAKE)
         for i in range(len(fractal)):
            label = init_plot_point(_plot_points)
            point = fractal[i]
            id = dpg.add_drag_point(default_value=(
                point['x'], point['y']), label=label, callback=point_callback,
                parent=plot_id, color=generate_color())
            _plot_points[label] = PlotPoint(id, point)
            dpg_set_point(_plot_points, id, label)
         out_x, out_y = get_series_from_list(fractal)
         _plot_result_series_id = dpg.add_line_series(out_x, out_y,  # type: ignore
                                                      label='Фрактал', parent=_axis_Oy_id)

      return __update_func

   @staticmethod
   def enable_items(disabled_items: list[int | str]):
      def __update_func(sender: int | str, _0=None, _1=None):
         for i in disabled_items:
            dpg.enable_item(i)
      return __update_func

   @staticmethod
   def disable_items(disabled_items: list[int | str]):
      def __update_func(sender: int | str, _0=None, _1=None):
         for i in disabled_items:
            dpg.disable_item(i)
      return __update_func


class PointHandlers:
   @staticmethod
   def print_points():
      def __update_func(sender: int | str, _0=None, _1=None):
         print("Points:")
         for i in list(_plot_points.values()):
            print(str(i.point), end="\n")
         print("---")
      return __update_func

   @staticmethod
   def update_points(plot_id: int | str):
      """Update plot points by point movement
      """
      _round_func = round_point(2)

      def __update_func(sender: int | str, app_data, user_data=None):
         global _plot_result_series_id, _axis_Oy_id, _plot_points
         dpg_init_oY_axis(plot_id)
         label = str(dpg.get_item_label(sender))
         # Value from point
         data = dpg.get_value(sender)
         new_x, new_y = data[0], data[1]
         _plot_points[label].id = sender
         _plot_points[label].point = l.Point(new_x, new_y)
         # rounded = _round_func(_plot_points[label].point)
         # x, y = rounded['x'], rounded['y']
         # dpg.set_value(sender, [x, y])
         dpg_set_point(_plot_points, sender, label)

      return __update_func

   @staticmethod
   def process_to_text(_, point_val): return "x: {:.2f},y: {:.2f}".format(
       round(point_val[0], 2), round(point_val[1], 2))


def generate_square_frame(round_func, point_size: int):
   points = []
   while True:
      p1 = get_unique_point(round_func, point_size, [])
      p2 = get_unique_point(round_func, point_size, [p1])
      if abs(p1['y'] - p2['y']) > 1:
         points = [p1, p2]
         break
   if points[0]['y'] > points[1]['y'] or points[0]['x'] > points[1]['x']:
      points.reverse()
   points.insert(1, l.Point(points[1]['x'], points[0]['y']))
   points.append(l.Point(points[0]['x'], points[2]['y']))
   return points


def init_plot_point(points: dict[str, PlotPoint]) -> str:
   global labels_alphabet
   start = 0
   for suffix in map(str, range(start-1, 10)):
      if suffix == str(start-1):
         suffix = ""
      for c in labels_alphabet:
         label = c + suffix
         if label in list(points.keys()):
            continue
         points[label] = PlotPoint()
         return label
   return ""
   # raise RuntimeError("Cannot generate label: maximum values exceeded")


def dpg_set_point(_plot_frame, id, label):
   if dpg_valid_id(id):
      real_val = dpg.get_value(id)
      _plot_frame[label].point.setName('x', real_val[0])
      _plot_frame[label].point.setName('y', real_val[1])


def dpg_init_oY_axis(plot_id: int | str):
   global _axis_Oy_id
   if _axis_Oy_id == 0:
      _axis_Oy_id = dpg.add_plot_axis(
          dpg.mvYAxis, label='y', parent=plot_id)


def dpg_try_delete_item(id: str | int):
   if dpg_valid_id(id):
      dpg.delete_item(id)


def dpg_valid_id(id):
   return len(str(id)) > 0 and str(id) != "0"


def get_unique_point(_round_func: Callable[[l.Point], l.Point], _point_size: int, points: list[l.Point]) -> l.Point:
   count, max_count = (0, 10)
   global point_min_val, point_max_val
   while True:
      p = l.Point.getRandom(point_min_val, point_max_val, _point_size)
      if count > max_count:
         break
      if _round_func(p) not in list(map(_round_func, points)):
         break
      count += 1
   return p


def generate_color():
   return random.choices(range(256), k=3)


def round_point(precision: int):
   def __round_func(value: l.Point) -> l.Point:
      return l.Point(round(value['x'], precision), round(value['x'], precision))
   return __round_func


def plot_objects_to_list(plot_objects: dict[str, PlotPoint]):
   result = []
   for i in list(plot_objects.values()):
      result.append(i.point)
   return result


def get_series_from_list(points: list[l.Point]) -> tuple[list[float], list[float]]:
   series_x, series_y = [], []
   for i in list(points):
      series_x.append(i['x'])
      series_y.append(i['y'])
   return series_x, series_y


def delete_series():
   global _plot_result_series_id, _plot_points
   dpg_try_delete_item(_plot_result_series_id)
   for i in list(_plot_points.values()):
      dpg_try_delete_item(i.id)
   _plot_result_series_id = 0
