# This file contain global vars
import lib_cppgeometry_wrapper as l
import dearpygui_ext.themes as dpg_ext


method_map_keys = [
    "Алгоритм Джарвиса (заворачивания подарка)", "Алгоритм Грэхема"]
method_map = {method_map_keys[0]: l.ConvexHullMethod.JARVIS,
              method_map_keys[1]: l.ConvexHullMethod.GRAHAM, }

# Default value
current_method = method_map_keys[0]

plot_number_amount = 5
point_min_val = 0
point_max_val = 3

themes = {"Черная": dpg_ext.create_theme_imgui_dark,
          "Светлая": dpg_ext.create_theme_imgui_light}
