import lib_cppgeometry_wrapper as l
import dearpygui_ext.themes as dpg_ext
import dearpygui.dearpygui as dpg
import gui


def window():
   def print_me(sender):
      print(f"Menu Item: {sender}")
   w = dpg.add_window(label="Tutorial")

   with dpg.menu_bar(parent=w):
      with dpg.menu(label="Сгенерировать точки"):
         dpg.add_menu_item(label="Save", callback=print_me)
         dpg.add_menu_item(label="Save As", callback=print_me)

         # with dpg.menu(label="Settings"):
         #    dpg.add_menu_item(label="Setting 1",
         #                      callback=print_me, check=True)
         #    dpg.add_menu_item(label="Setting 2", callback=print_me)

      dpg.add_menu_item(label="Построить выпуклую оболочку", callback=print_me)

      dpg.add_menu_item(label="Помощь")
   dpg.set_primary_window(w, True)


def viewport():
   dpg.create_viewport(title='ConvexHull GUI',
                       width=800, height=600)


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
