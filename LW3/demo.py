import gui
import dearpygui.demo as d
import dearpygui.dearpygui as dpg

def demo():
   d.show_demo()
   dpg.set_primary_window("__demo_id", True)

gui.create_gui(gui.Parameters([demo],None,None,None),True)
