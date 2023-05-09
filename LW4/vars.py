# This file contain global variables
import lib_cppgeometry_wrapper as l
import dearpygui_ext.themes as dpg_ext

debug = True


method_map = {"Алгоритм Сазерленда-Коэна": l.ClipSegmentMethod.COHEN_SUTHERLAND,
              "Алгоритм Спрулла-Сазерленда": l.ClipSegmentMethod.SPROULE_SUTHERLAND,
              "Алгоритм Кируса-Бека": l.ClipSegmentMethod.CYRUS_BECK}
# Default value
current_method = "Алгоритм Сазерленда-Коэна"
plot_number_amount = 4
point_min_val = 0
point_max_val = 10

themes = {"Черная": dpg_ext.create_theme_imgui_dark,
          "Светлая": dpg_ext.create_theme_imgui_light}
