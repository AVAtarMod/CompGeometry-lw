import sys
import dearpygui.demo as d
import dearpygui.dearpygui as dpg
from pathlib import Path
import importlib

def import_parents(level=1):
   global __package__
   file = Path(__file__).resolve()
   parent, top = file.parent, file.parents[level]

   sys.path.append(str(top))
   try:
      sys.path.remove(str(parent))
   except ValueError:  # already removed
      pass

   __package__ = '.'.join(parent.parts[len(top.parts):])
   importlib.import_module(__package__)  # won't be needed after that


if __name__ == '__main__' and __package__ is None or __package__ == '':
   import_parents(level=2)
   from ..lib.gui import themes as t
   from ..lib.gui import gui
   from ..lib.gui.events import EventHandlerPool


def demo():
   d.show_demo()
   dpg.set_primary_window("__demo_id", True)

gui.create_gui(gui.Parameters([demo],None,None,None),True)
