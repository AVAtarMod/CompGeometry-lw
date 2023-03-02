import lib_cppgeometry_wrapper as l

for i in dir(l):
   print(i, "  ", type(getattr(l, i)))
