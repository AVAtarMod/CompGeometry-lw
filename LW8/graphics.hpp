#include <fstream>
#include <iostream>
#include <vector>

#include "lib_cppgeometry/Angle.hpp"
#include "lib_cppgeometry/ComplexNumber.hpp"
#include "lib_cppgeometry/Fractals.hpp"
#include "lib_cppgeometry/Point.hpp"
#include "lib_cppgeometry/functions.hpp"

#include "third-party/CImg.h"

using namespace cimg_library;
void clearScreen();
void printMandelbrotSet();
void printNewtonFractal();
void printPlasmaFractal();
void printCochSnowflakeFractal();
