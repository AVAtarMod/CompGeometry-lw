import lib_cppgeometry_wrapper as l
import dearpygui_ext.themes as dpg_ext
import dearpygui.dearpygui as dpg
import gui

def update_text(item_id):
   def __update(sender):
      val = dpg.get_value(sender)
      dpg.set_value(item_id, "x: {:.2f},y: {:.2f}".format(
          round(val[0],2), round(val[1],2)))
   return __update

def window():
   labels = "ABCDEFGHIJKLMPRSTUFXYZ"
   used = []

   def print_me(sender):
      print(f"Menu Item: {sender}")
   w = dpg.add_window(label="main window")

   with dpg.menu_bar(parent=w):
      dpg.add_menu_item(label="Сгенерировать точки")

      dpg.add_menu_item(label="Построить выпуклую оболочку", callback=print_me)

      with dpg.menu(label="Настройки"):
         dpg.add_menu_item(label="Setting 1",
                           callback=print_me, check=True)
         dpg.add_menu_item(label="Setting 2", callback=print_me)

      dpg.add_menu_item(label="Помощь")
   p = l.Point.getRandom(0, 3, 2)
   x,y = p["x"],p["y"]
   text = dpg.add_text(f"x: {x},y: {y}",parent=w)
   with dpg.plot(label="График", height=400, width=-1, parent=w):
      dpg.add_plot_legend()
      oX = dpg.add_plot_axis(dpg.mvXAxis, label="x")
      oY = dpg.add_plot_axis(dpg.mvYAxis, label="y")

      # drag lines/points belong to the plot NOT axis
      # dpg.add_drag_line(
      #    label="dline1", color=[255, 0, 0, 255])
      # dpg.add_drag_line(label="dline2", color=[
      #    255, 255, 0, 255], vertical=False)

      dpg.add_drag_point(
         label="dpoint1", color=[255, 0, 255, 255],default_value=(p["x"],p["y"]),callback=update_text(text))
      # dpg.add_drag_point(
      #    label="dpoint2", color=[255, 0, 255, 255])
   dpg.set_primary_window(w, True)


def viewport():
   dpg.create_viewport(title='ConvexHull GUI',
                       width=1000, height=600)


def fix_font(id: str | int):
   dpg.add_font_range_hint(dpg.mvFontRangeHint_Cyrillic, parent=id)


def main():
   theme = dpg_ext.create_theme_imgui_light
   font = ["assets/Montserrat-Regular.otf", 25]

   items = [window, viewport]
   params = gui.Parameters(items, theme, font[0], fix_font, font[1])
   gui.create_gui(params)


if __name__ == "__main__":
   main()
