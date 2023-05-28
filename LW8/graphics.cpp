#include "graphics.hpp"

CImg<unsigned char> vectorToImg(const std::vector<std::vector<RGB>>& vec);
void controlToCartesian(const Point& p, Point& p1);
double controlToCartesianX(double x);
double controlToCartesianY(double y);
void focusDif(double dx, double dy);
void mouseWheelHandler();

void clearScreen()
{
#if defined(_MSC_VER) || defined(WIN32) || defined(_WIN32) ||                  \
  defined(__WIN32__) || defined(WIN64) || defined(_WIN64) ||                   \
  defined(__WIN64__)
   system("cls");
#elif defined(unix) || defined(__unix) || defined(__unix__) ||                 \
  defined(linux) || defined(__linux) || defined(__linux__)
   system("clear");
#endif
}

Point vert(-3, 1.125);
int width_px = 1280, height_px = 720;
Point center(width_px / 2, height_px / 2);

double width = 4, height = 2.25, x = 0, y = 0, h = 320, prev_h = 320;
Point focus(-1, 0);

int max_iterations = 1000, m_x, m_y;

Point mouse = Point(0, 0);
int wheel = 0, prev_wheel = 0;

CImg<unsigned char> img;
CImgDisplay disp;

Point buf(0, 0);

void printMandelbrotSet()
{
   img = vectorToImg(Fractals::mandelbrotSet(
     vert, width_px, height_px, width, height, max_iterations));
   disp = CImgDisplay(img, " ", false);

   while (!disp.is_closed()) {
      m_x = disp.mouse_x();
      m_y = disp.mouse_y();
      mouse[0] = m_x;
      mouse[1] = m_y;
      wheel = disp.wheel();
      if (wheel - prev_wheel != 0) {
         prev_wheel = wheel;
         mouseWheelHandler();
         img = vectorToImg(Fractals::mandelbrotSet(
           vert, width_px, height_px, width, height, max_iterations));
      }
      clearScreen();
      controlToCartesian(mouse, buf);
      std::cout << buf << std::endl;
      disp.display(img);
   }
}

void printNewtonFractal()
{
   vert = Point(-2, 1.125);
   focus = Point(0, 0);
   img = vectorToImg(
     Fractals::NewtonFractal(vert, width_px, height_px, width, height));
   disp = CImgDisplay(img, " ", false);

   while (!disp.is_closed()) {
      m_x = disp.mouse_x();
      m_y = disp.mouse_y();
      mouse[0] = m_x;
      mouse[1] = m_y;
      wheel = disp.wheel();
      if (wheel - prev_wheel != 0) {
         prev_wheel = wheel;
         mouseWheelHandler();
         img = vectorToImg(
           Fractals::NewtonFractal(vert, width_px, height_px, width, height));
      }
      clearScreen();
      controlToCartesian(mouse, buf);
      std::cout << buf << std::endl;
      disp.display(img);
   }
}

void printPlasmaFractal()
{
   img = vectorToImg(Fractals::plasmaFractal(9));
   disp = CImgDisplay(img, " ", false);

   std::cout << "Press X to next image." << std::endl;
   while (!disp.is_closed()) {
      if (disp.button() & 1) {
         img = vectorToImg(Fractals::plasmaFractal(9));
         disp.display(img);
         disp.wait(500);
      } else
         disp.display(img);
   }
}

CImg<unsigned char> vectorToImg(const std::vector<std::vector<RGB>>& vec)
{
   CImg<unsigned char> img(vec[0].size(), vec.size(), 1, 3);
   cimg_forXY(img, x, y)
   {
      img(x, y, 0, 0) = vec[y][x]._red;
      img(x, y, 0, 1) = vec[y][x]._green;
      img(x, y, 0, 2) = vec[y][x]._blue;
   }
   return img;
}

void controlToCartesian(const Point& p, Point& p1)
{
   p1[0] = controlToCartesianX(p[0]);
   p1[1] = controlToCartesianY(p[1]);
}

double controlToCartesianX(double x)
{
   return (x - center[0]) / h + focus[0];
}

double controlToCartesianY(double y)
{
   return (center[1] - y) / h + focus[1];
}

void focusDif(double dx, double dy)
{
   focus[0] += dx;
   focus[1] += dy;
   vert[0] = focus[0] - width / 2;
   vert[1] = focus[1] + height / 2;
}

void mouseWheelHandler()
{
   Point p = Point(0, 0);
   controlToCartesian(mouse, p);

   h = 320 * pow(1.1, wheel);
   width = width_px / h;
   height = height_px / h;

   Point p1 = Point(0, 0);
   controlToCartesian(mouse, p1);
   focusDif(p[0] - p1[0], p[1] - p1[1]);
}
