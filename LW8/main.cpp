#include "graphics.hpp"

int main(int argc, char* argv[])
{
   int ind = -1;
   std::cout << "(1) Mandelbrot set\n"
             << "(2) Newton fractal\n"
             << "(3) Plasma fractal\n"
             << "(4) Koch Snowflake fractal\n"
             << ">> ";
   std::cin >> ind;
   clearScreen();
   switch (ind) {
      case 1:
         printMandelbrotSet();
         break;
      case 2:
         printNewtonFractal();
         break;
      case 3:
         printPlasmaFractal();
         break;
      case 4:
         printCochSnowflakeFractal();
         break;
   }
}
