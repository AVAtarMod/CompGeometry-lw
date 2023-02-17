#include <fstream>
#include <iostream>
#include <vector>

#include "lib_cppgeometry/Point.hpp"
#include "lib_cppgeometry/LineSegment.hpp"

void out(const Point& a)
{
    std::cout << a[0] << " " << a[1] << std::endl;
}

void task1_1()
{
    Point p1(0.8, 0);
    Point p2(2, 6);
    Point p3(0.456, -1.72);
    //Point ans = Point();
    //ans += p1;
    //ans += -p2;
    //out(ans);
    //out(p1);
    //out(p2);
    //p3 = p1 - p2;
    double a1, a2, a3;
    //a1 = p1 * p2 / (p1.Length() * p2.Length());
    //p1 -= p3;
    //p2 -= p3;
    //a2 = p1 * p2 / (p1.Length() * p2.Length());
    a1 = Point::angle(p1, p2, p3) * 180 / acos(-1.0);
    a2 = Point::angle(p2, p3, p1) * 180 / acos(-1.0);
    a3 = Point::angle(p3, p1, p2) * 180 / acos(-1.0);
    std::cout << a1 << std::endl << a2 << std::endl << a3 << std::endl;
}

namespace t1_1
{
    void read(std::ifstream& in, std::vector<Point>& out)
    {
        size_t testsCount = 0;
        in >> testsCount;
        const size_t testSize = 3;
        out = std::vector<Point>(testsCount * testSize);
        for (size_t i = 0; i < testsCount; ++i)
        {
            for (size_t iPoint = 0; iPoint < 3; ++iPoint)
            {
                int x, y;
                in >> x >> y;
                out[i * testSize + iPoint] = Point(x, y);
            }
        }
    }
    void solve(std::ofstream& out, std::vector<Point>& in)
    {
    }

} // namespace t1_1

void task_1()
{
    const char* in_file = "task2_1in.txt";
    const char* out_file = "task2_1out.txt";

    std::ifstream in(in_file);
    std::ofstream out(out_file);

    if (!in)
    {
        std::cerr << "File " << in_file << " not exist or not accessible\n";
    }
    else if (!out)
    {
        std::cerr << "File " << out_file << " not exist or not accessible\n";
    }

    else
    {
        std::vector<Point> tests;
        t1_1::read(in, tests);
        t1_1::solve(out, tests);
    }
    out.close();
    in.close();
}
void task1_2()
{
    std::ifstream in("task2_2in.txt");
}