import random
import dearpygui.dearpygui as dpg
import lib_cppgeometry_wrapper as l
from vars import *
from typing import Callable
import dataclasses
from dataclasses import dataclass

# TODO: round point coordinates to 2 digits (need use dpg.set_value)

labels_alphabet = "ABCDEFGHIJKLMPRSTUFXYZ"
_max_points = len(labels_alphabet)*10
_axis_Oy_id = 0
null_tag = "null_tag"
default_plot_na = plot_number_amount


@dataclass
class PlotPoint:
   id: int | str = 0
   point: l.Point = dataclasses.field(default_factory=l.Point(0, 0))

   def __init__(self, id: int | str = 0, point: l.Point = l.Point(0, 0)) -> None:
      self.id = id
      self.point = point


_plot_clip_series = {"raw": 0, "out": 0}
_plot_clip_val = {str(): PlotPoint()}
_plot_clip_points = {"raw": _plot_clip_val,
                     "out": _plot_clip_val}
"""Dictionary with structure {"clip area type": [(id: int, point: l.Point)]}
"""

_plot_frame_series_id = 0
_plot_frame = {}
"""Dictionary with structure {"label": PlotPoint}
"""
frame_point_suffix = 'b_'
clip_point_suffix = 'c_'


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
   def set_plot_number_amount(method_map_listbox_id: int | str):
      def __update_func(sender: int | str, _0=None, _1=None):
         strval = dpg.get_value(sender)
         global plot_number_amount, _max_points

         val = int(strval) if strval != '' else plot_number_amount
         if val != default_plot_na:
            method_list = list(method_map.keys())[2]
            ListBoxHandlers.set_current_method(0, method_list)
            dpg.configure_item(method_map_listbox_id, items=[method_list])
         else:
            dpg.configure_item(method_map_listbox_id,
                               items=list(method_map.keys()))
         if val > _max_points:
            dpg.set_value(sender, plot_number_amount)
         else:
            plot_number_amount = int(val)
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
   def clip_line(notify_procedure: Callable[[str], None], plot_id: int | str):
      def __update_func(sender: int | str, _0=None, _1=None):
         global _plot_clip_series, _plot_clip_points, _axis_Oy_id, _plot_clip_points, _plot_frame
         raw_points = [i.point for i in list(_plot_clip_points['raw'].values())]
         points = [i.point for i in list(_plot_frame.values())]
         polygon = l.Polygon(l.VectorPoint(
             points))
         raw_segment = l.LineSegment(raw_points[0], raw_points[1])
         out_segment = polygon.segmentInsidePolygon(
             raw_segment, method_map[current_method])

         ui_clear_box(_plot_clip_points['out'], _plot_clip_series['out'])
         _plot_clip_series['out'] = 0

         if out_segment is not None:
            out_points = [out_segment.getBegin(), out_segment.getEnd()]
            label_begin = clip_point_suffix + 'begin'
            label_end = clip_point_suffix + 'end'
            out_begin = dpg.add_drag_point(default_value=(
                out_points[0]['x'], out_points[0]['y']), label='Clip begin',
                parent=plot_id, color=generate_color(), user_data=label_begin)
            out_end = dpg.add_drag_point(default_value=(
                out_points[1]['x'], out_points[1]['y']), label='Clip end',
                parent=plot_id, color=generate_color(), user_data=label_end)

            _plot_clip_points['out'] = {label_begin: PlotPoint(int(out_begin), out_points[0]),
                                        label_end: PlotPoint(int(out_end), out_points[1])}
            out_x, out_y = get_series_from_list(
                plot_objects_to_list(_plot_clip_points['out']))
            _plot_clip_series['out'] = (dpg.add_line_series(out_x, out_y,  # type: ignore
                                                            label='Обрезанная линия', parent=_axis_Oy_id))
            notify_procedure("Отрезок успешно обрезан")
         else:
            msg = f"Линия {str(raw_points[0])} -> {str(raw_points[1])} вне границ области отрисовки."
            notify_procedure(msg)
      return __update_func

   @staticmethod
   def generate_frame(plot_id: int | str, frame_point_callback):
      _round_func = round_point(2)
      _point_size = 2

      def __update_func(sender: int | str, _0=None, _1=None):
         global _plot_frame_series_id, _plot_frame
         global point_min_val, point_max_val
         ui_clear_box(_plot_frame, _plot_frame_series_id, null_tag)
         _plot_frame_series_id = 0
         _plot_frame.clear()
         _points = []
         dpg_init_oY_axis(plot_id)
         if plot_number_amount == 4:
            _points = generate_square_frame(_round_func, _point_size)
         else:
            for _ in range(plot_number_amount):
               p = get_unique_point(_round_func, _point_size, _points)
               _points.append(p)
         for p in _points:
            label = allocate_plot_object(_plot_frame)
            _plot_frame[label] = PlotPoint(0, p)
         generate_frame_box(_plot_frame, plot_id, frame_point_callback)

      return __update_func

   @staticmethod
   def enable_items(disabled_items: list[int | str]):
      def __update_func(sender: int | str, _0=None, _1=None):
         for i in disabled_items:
            dpg.enable_item(i)
      return __update_func

   @staticmethod
   def generate_clip_line(plot_id: int | str):
      _round_func = round_point(2)
      _point_size = 2

      def __update_func(sender: int | str, _0=None, _1=None):
         global _axis_Oy_id, _plot_clip_series, _plot_clip_points
         global point_min_val, point_max_val
         raw = {}
         for i in list(_plot_clip_points.values()):
            for point_data in list(i.values()):
               dpg_try_delete_item(point_data.id)
               point_data.id = 0
            i.clear()

         for label in _plot_clip_series:
            dpg_try_delete_item(_plot_clip_series[label])
            _plot_clip_series[label] = 0

         for _ in range(2):
            label = allocate_plot_object(raw)
            raw[label].point = get_unique_point(
                _round_func, _point_size, plot_objects_to_list(raw))
            raw[label].id = dpg.add_drag_point(default_value=(
                raw[label].point['x'], raw[label].point['y']), label=label, callback=PointHandlers.update_clip_lines(),
                parent=plot_id, color=generate_color(), user_data=clip_point_suffix + label)
         x, y = get_series_from_list(plot_objects_to_list(raw))
         _plot_clip_points['raw'] = raw
         _plot_clip_series['raw'] = dpg.add_line_series(x, y,  # type: ignore
                                                        label='Исходная линия', parent=_axis_Oy_id)

      return __update_func


class PointHandlers:
   @staticmethod
   def print_points():
      def __update_func(sender: int | str, _0=None, _1=None):
         print("Clip points:")
         for i in _plot_clip_points:
            print("type:", i)
            for data in _plot_clip_points[i]:
               print("\t", str(_plot_clip_points[i][data].point), end="\n")
         print("Frame points:")
         for i in list(_plot_frame.values()):
            print(str(i.point), end="\n")
         print("---")
      return __update_func

   @staticmethod
   def update_frame(plot_id: int | str, frame_point_callback):
      """Update lines of frame by point movement
      """
      def __update_func(sender: int | str, app_data, user_data=None):
         global _plot_frame_series_id, _axis_Oy_id
         dpg_init_oY_axis(plot_id)
         label = str(dpg.get_item_configuration(sender)['user_data'])
         # Value from point
         data = dpg.get_value(sender)
         new_x, new_y = data[0], data[1]
         if label[0:2] == frame_point_suffix:
            label = label[2:]
            _plot_frame[label].id = sender
            _plot_frame[label].point = l.Point(new_x, new_y)
            if plot_number_amount == 4:
               correct_square_frame(_plot_frame, label)
            generate_frame_box(_plot_frame, plot_id,
                               frame_point_callback, sender)
         else:
            raise RuntimeError(
                "Cannot update frame if sender is non-frame point.")
      return __update_func

   @staticmethod
   def update_clip_lines():
      """Update clip lines by point movement
      """
      def __update_func(sender: int | str, app_data, user_data=None):
         global _plot_clip_series, _plot_clip_points, _axis_Oy_id
         label = str(user_data)
         real_label = label[2:]
         if label[0:2] == clip_point_suffix:
            ui_clear_box(_plot_clip_points['out'],
                         _plot_clip_series['out'], sender)
            for series in _plot_clip_series:
               dpg_try_delete_item(_plot_clip_series[series])
               _plot_clip_series[series] = 0
            # Value from point
            data = dpg.get_value(sender)
            new_x, new_y = data[0], data[1]
            changed = _plot_clip_points['raw'][real_label]
            changed.point = l.Point(new_x, new_y)
            changed.id = sender
            new_x, new_y = get_series_from_list(
                plot_objects_to_list(_plot_clip_points['raw']))
            _plot_clip_series['raw'] = dpg.add_line_series(new_x, new_y,  # type: ignore
                                                           label='Исходная линия', parent=_axis_Oy_id)
         else:
            raise RuntimeError(
                "Cannot update frame if sender is non-frame point.")
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


def allocate_plot_object(points: dict[str, PlotPoint]):
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
   raise RuntimeError("Cannot generate label: maximum values exceeded")


def ui_clear_box(_plot_frame: dict[str, PlotPoint], series_id: int | str, exclude: int | str = null_tag):
   for i in list(_plot_frame.values()):
      if exclude != null_tag and i.id == exclude:
         continue
      dpg_try_delete_item(i.id)
      i.id = 0
   dpg_try_delete_item(series_id)


def generate_frame_box(_plot_frame: dict[str, PlotPoint], plot_id: int | str, frame_point_callback, exclude: int | str = null_tag):
   global _plot_frame_series_id
   id = _plot_frame_series_id
   _points = plot_objects_to_list(_plot_frame)
   x, y = get_series_from_list(_points)
   ui_clear_box(_plot_frame, id, exclude)
   for label in _plot_frame:
      dpg_set_point(_plot_frame, _plot_frame[label].id, label)
      if _plot_frame[label].id == exclude:
         continue

      tag = frame_point_suffix + label
      p = _plot_frame[label].point
      id = dpg.add_drag_point(default_value=(
          p['x'], p['y']), label='Точка области', callback=frame_point_callback,
          parent=plot_id, color=generate_color(), user_data=tag)
      _plot_frame[label].id = id
      dpg_set_point(_plot_frame, id, label)
   _plot_frame_series_id = dpg.add_line_series(
       x, y, label='Область', parent=_axis_Oy_id)


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
   global point_min_val, point_max_val, _plot_frame
   while True:
      p = l.Point.getRandom(point_min_val, point_max_val, _point_size)
      if count > max_count:
         break
      if _round_func(p) not in list(map(_round_func, points)):
         break
      count += 1
   return p


def correct_square_frame(plot_frame: dict[str, PlotPoint], label: str) -> None:
   length = len(plot_frame)
   l = [i.point for i in list(plot_frame.values())]
   i = list(plot_frame.keys()).index(label)

   begin = (i + 1) % length
   end = (i - 1) % length
   # 0 = 'x', 1 = 'y'
   begin_coord = (i + 1) % 2
   end_coord = i % 2
   l[begin].setIndex(begin_coord, l[i].getIndex(begin_coord))
   l[end].setIndex(end_coord, l[i].getIndex(end_coord))

   for i, key in enumerate(plot_frame):
      plot_frame[key].point = l[i]


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
   if len(points) > 0:
      series_x.append(series_x[0])
      series_y.append(series_y[0])
   return series_x, series_y
