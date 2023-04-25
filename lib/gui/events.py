from typing import Callable
from inspect import signature as sig
import dearpygui.dearpygui as dpg


class EventHandlerPool:
   handlers: list[Callable[..., None]]
   item_id: int | str
   callback_was_set: bool = False

   def __init__(self, item_id: int | str = 0, set_callback: bool = True) -> None:
      self.handlers = []
      self.item_id = item_id

      if set_callback:
         dpg.set_item_callback(self.item_id, callback=self)
         self.callback_was_set = True

   def set_item(self, new_item_id: int | str, set_callback: bool = True):
      if self.callback_was_set:
         dpg.set_item_callback(self.item_id, lambda: None)
      self.item_id = new_item_id
      if set_callback:
         dpg.set_item_callback(self.item_id, callback=self)

   def __iadd__(self, handler: Callable[..., None]):
      s = sig(handler)
      parameters_count = len(s.parameters)
      if parameters_count < 1 or parameters_count > 3:
         raise TypeError("Only handlers with 1-3 parameters supported")

      self.handlers.append(handler)
      return self

   def __add__(self, handler: Callable[..., None]):
      s = sig(handler)
      parameters_count = len(s.parameters)
      if parameters_count < 1 or parameters_count > 3:
         raise TypeError("Only handlers with 1-3 parameters supported")

      result = EventHandlerPool(self.item_id)
      result.handlers.extend(self.handlers)
      result.handlers.append(handler)
      return result

   def __call__(self, *args, **kwargs) -> None:
      for i in self.handlers:
         i(*args, **kwargs)

   def get_bind_handler(self, external_item_id: str | int, procedure: Callable[[object, object], object] | None = None, use_sender: bool = False):
      """Get handler that change value item of this EventHandlerPool
      by value of `external_item_id`.

      Args:
         external_item_id (str | int): source of data
         procedure (Callable[[object, object], None]): function to process value from `external_item_id`.
         Must accept 
         - current value of `self` item as first parameter,
         - current value of `external_item_id` as second parameter

         use_sender (bool): use sender value in resulted handler instead of `external_item_id`

      Returns:
         event handler. May be appended to EventHandlerPool for `external_item_id`
      """
      if procedure is None:
         def __update(sender, _0=None, _1=None):
            """Generated handler
            """
            ext_val = dpg.get_value(
                sender) if use_sender else dpg.get_value(external_item_id)
            dpg.set_value(self.item_id, ext_val)
      else:
         def __update(sender, _0=None, _1=None):
            """Generated handler
            """
            val = dpg.get_value(self.item_id)
            ext_val = dpg.get_value(
                sender) if use_sender else dpg.get_value(external_item_id)
            out = procedure(val, ext_val)
            dpg.set_value(self.item_id, out)
      return __update
