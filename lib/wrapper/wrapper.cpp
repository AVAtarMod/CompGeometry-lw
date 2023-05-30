#include "lib_cppgeometry/Curve.hpp"
#include "lib_cppgeometry/Fractals.hpp"
#include "lib_cppgeometry/Point.hpp"
#include "lib_cppgeometry/Polygon.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#ifndef NDEBUG
#define PYBIND11_DETAILED_ERROR_MESSAGES
#endif // NDEBUG

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<Point>);
PYBIND11_MAKE_OPAQUE(std::pair<double, double>);

void pp(std::pair<int, int> p)
{
   std::cout << "pp: p is ( " << p.first << " " << p.second << ")\n";
}

std::unique_ptr<LineSegment> clipSegment(Polygon* this_, const LineSegment& ls,
                                         const Polygon::ClipSegmentMethod& m)
{

   std::cout << "WRAPPER clipSegment: Received:\n\t";
   std::cout << ls.getBegin() << "\n\t" << ls.getEnd() << "\n";

   auto result = this_->segmentInsidePolygon(ls, m);

   std::cout << "WRAPPER clipSegment: Result:\n\t";
   if (result == nullptr)
      std::cout << "Null\n";
   else {
      std::cout << (*result).getBegin() << "\n\t" << (*result).getEnd() << "\n";
   }
   return result;
}
/**
static std::vector<Point> Fractals::geometricFractal(const Point &p, const
Fractals::Area &area, Fractals::GeometricFractalType t)
 */
std::vector<Point> geometricFractal(const Point& p, size_t w, size_t h,
                                    double min_x, double max_x, double min_y,
                                    double max_y,
                                    Fractals::GeometricFractalType t)
{
   std::cout << "WRAPPER geometricFractal: Received:\n\t";
   std::cout << p << " h_px=" << h << " w_px=" << w << " x=(" << min_x << ", "
             << max_x << ") y=(" << min_y << ", " << max_y << ")"
             << "\n";

   auto result = Fractals::geometricFractal(
     p, Fractals::Area(w, h, { min_x, max_x }, { min_y, max_y }), t);

   std::cout << "WRAPPER geometricFractal: Result:\n\t";
   if (result.size() == 0)
      std::cout << "Null\n";
   else {
      std::cout << "Vector, size =" << result.size() << "\n";
   }

   return result;
}

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
          py::overload_cast<const std::string&>(&Point::operator[], py::const_))
     .def("getIndex", py::overload_cast<size_t>(&Point::operator[], py::const_))
     .def("setIndex",
          [](Point& self, size_t index, double value) { self[index] = value; })
     .def("getName",
          py::overload_cast<const std::string&>(&Point::operator[], py::const_))
     .def("setName",
          [](Point& self, const std::string& name, double value) {
             self[name] = value;
          })
     .def("__str__", &Point::to_string)
     .def("__eq__", &Point::operator==)
     .def("pp", &pp);

   py::class_<LineSegment>(m, "LineSegment")
     .def(py::init<const Point&, const Point&>())
     .def(py::init<const Line&, const Point*>())
     .def("getBegin", &LineSegment::getBegin)
     .def("getEnd", &LineSegment::getEnd);

   py::class_<Polygon>(m, "Polygon")
     .def(py::init<const std::vector<Point>&>(), "Construct N-point Polygon")
     .def("convexHull", &Polygon::convexHull)
     .def("segmentInsidePolygon", &clipSegment)
     .def("size", &Polygon::size)
     .def("__len__", &Polygon::size)
     .def("get", &Polygon::get)
     .def("__setitem__", py::overload_cast<int>(&Polygon::operator[]));

   py::enum_<Polygon::ConvexHullMethod>(m, "ConvexHullMethod")
     .value("GRAHAM", Polygon::ConvexHullMethod::GRAHAM)
     .value("JARVIS", Polygon::ConvexHullMethod::JARVIS)
     .export_values();

   py::enum_<Polygon::ClipSegmentMethod>(m, "ClipSegmentMethod")
     .value("COHEN_SUTHERLAND", Polygon::ClipSegmentMethod::COHEN_SUTHERLAND)
     .value("SPROULE_SUTHERLAND",
            Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND)
     .value("CYRUS_BECK", Polygon::ClipSegmentMethod::CYRUS_BECK)
     .export_values();

   py::class_<Curve>(m, "Curve")
     .def(py::init<const std::vector<Point>&>())
     .def("size", &Curve::size)
     .def("__len__", &Curve::size)
     .def("__getitem__", &Curve::operator[])
     .def("makeBezierCurveQuadratic",
          py::overload_cast<const Point&, const Point&, const Point&>(
            &Curve::makeBezierCurve))
     .def(
       "makeBezierCurveCubic",
       py::
         overload_cast<const Point&, const Point&, const Point&, const Point&>(
           &Curve::makeBezierCurve));

   py::class_<Fractals>(m, "Fractals")
     .def("geometricFractal", &geometricFractal);

   py::enum_<Fractals::GeometricFractalType>(m, "GeometricFractalType")
     .value("KOCH_SNOWFLAKE", Fractals::GeometricFractalType::KOCH_SNOWFLAKE)
     .value("PYTHAGORAS_TREE_CLASSIC",
            Fractals::GeometricFractalType::PYTHAGORAS_TREE_CLASSIC)
     .value("PYTHAGORAS_TREE_NAKED",
            Fractals::GeometricFractalType::PYTHAGORAS_TREE_NAKED)
     .export_values();

   py::bind_vector<std::vector<Point>>(m, "VectorPoint");
}
