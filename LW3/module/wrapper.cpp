#include "lib_cppgeometry/Point.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(lib_cppgeometry_wrapper, m)
{
   m.doc() = "lib_cppgeometry wrapper for Python"; // optional module docstring

   py::class_<Point>(m, "Point")
     .def(py::init<int>())
     .def(py::init<const Point&>())
     .def(py::init<double, double>())
     .def("size", &Point::size)
     .def("dimension", &Point::size)
     .def("getRandom", &Point::getRandom);
}
