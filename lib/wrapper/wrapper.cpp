#include "lib_cppgeometry/Point.hpp"
#include "lib_cppgeometry/Polygon.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

std::vector<Point> getConvexHull(const std::vector<Point>& points,
                                 const ConvexHullMethod& m)
{

#ifndef NDEBUG
   std::cout << "Received:\n";
   for (auto&& i : points) {
      std::cout << i << "\n";
   }
#endif

   auto result = Polygon::convexHull(points, m).get();

#ifndef NDEBUG
   std::cout << "Convex hull:\n";
   for (auto&& i : result) {
      std::cout << i << "\n";
   }
#endif

   return result;
}

PYBIND11_MAKE_OPAQUE(std::vector<Point>);

PYBIND11_MODULE(lib_cppgeometry_wrapper, m)
{
   m.doc() = "lib_cppgeometry wrapper for Python"; // optional module docstring

   py::class_<Point>(m, "Point")
     .def(py::init<int>(), "Construct N-dimension Point")
     .def(py::init<const Point&>(), "Copy constructor")
     .def(py::init<double, double>(), "Construct 2-dimension Point")
     .def("size", &Point::size)
     .def("dimension", &Point::size)
     .def("getRandom", &Point::getRandom)
     .def("__getitem__",
          py::overload_cast<const char*>(&Point::operator[], py::const_))
     .def("__setitem__", py::overload_cast<const char*>(&Point::operator[]))
     .def("__str__", &Point::to_string)
     .def("__eq__",&Point::operator==);

   py::class_<Polygon>(m, "Polygon")
     .def(py::init<const std::vector<Point>&>(), "Construct N-point Polygon")
     .def("convexHull", &getConvexHull)
     .def("size", &Polygon::size)
     .def("__len__", &Polygon::size)
     .def("get", &Polygon::get)
     .def("__setitem__", py::overload_cast<int>(&Polygon::operator[]));

   py::enum_<ConvexHullMethod>(m, "ConvexHullMethod")
     .value("GRAHAM", ConvexHullMethod::GRAHAM)
     .value("JARVIS", ConvexHullMethod::JARVIS)
     .export_values();

   py::bind_vector<std::vector<Point>>(m, "VectorPoint");
}