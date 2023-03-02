#include <pybind11/pybind11.h>
#include "Point.hpp"
#include ""

namespace py = pybind11;

PYBIND11_MODULE(lib_cppgeometry_wrapper, m)
{
   m.doc() = "pybind11 example plugin"; // optional module docstring

   m.def("add",
         &add,
         "A function that adds two numbers",
         py::arg("i"),
         py::arg("j"));
}
