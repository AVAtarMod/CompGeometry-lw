#include <fstream>
#include <iostream>
#include <vector>

#include "lib_cppgeometry/Point.hpp"
#include "lib_cppgeometry/LineSegment.hpp"

std::vector<Point> read(std::ifstream& in)
{
    int testsCount;
    double x, y;
    in >> testsCount;
    std::vector<Point> out(testsCount);
    for (int i = 0; i < testsCount; i++)
    {
        in >> x >> y;
        out[i] = Point(x, y);
    }
    return out;
}

bool isZero(double a)
{
    return fabs(a) <= std::numeric_limits<double>::epsilon();
}

int sign(double a)
{
    if (isZero(a)) return 0;
    else if (a < 0) return -1;
    else return 1;
}

namespace t1_1
{
    bool isLine(const std::vector<Point> points)
    {
        double a = Point::angle(points[0], points[1], points[2]) * 180 / acos(-1.0);
        if (isZero(a) ||
            isZero(a - 180))
            return true;
        else return false;
    }
}

void task1_1()
{
    const char* in_file = "task1_1in.txt";
    std::ifstream in(in_file);

    if (!in)
    {
        std::cerr << "File " << in_file << " not exist or not accessible\n";
    }
    else
    {
        std::vector<Point> points = read(in);
        std::cout << t1_1::isLine(points) << std::endl;
    }
    in.close();
}

namespace t1_2
{
    int cycle(std::vector<Point> points)
    {
        Point p1 = (points[0] - points[2]) ^ (points[1] - points[2]);
        Point p2 = (points[1] - points[0]) ^ (points[2] - points[0]);
        Point p3 = (points[2] - points[1]) ^ (points[0] - points[1]);
        if (p1[2] < 0 && p2[2] < 0 && p3[2] < 0) return -1;
        else if (p1[2] > 0 && p2[2] > 0 && p3[2] > 0) return 1;
        else return 0;
    }
}

void task1_2()
{
    const char* in_file = "task1_2in.txt";
    std::ifstream in(in_file);

    if (!in)
    {
        std::cerr << "File " << in_file << " not exist or not accessible\n";
    }
    else
    {
        std::vector<Point> points = read(in);
        int i = t1_2::cycle(points);
        if (i == 1) std::cout << "Против часовой" << std::endl;
        else if (i == -1) std::cout << "По часовой" << std::endl;
    }
    in.close();
}

namespace t1_3
{
    bool equal(double a, double b) //Точки находятся по одну сторону от отрезка?
    {
        if (isZero(a) || isZero(b) || (sign(a) == sign(b))) return true;
        else return false;
    }

    bool isIntersection(std::vector<Point> points)
    {
        double p1 = ((points[1] - points[0]) ^ (points[2] - points[0]))[2];
        double p2 = ((points[2] - points[0]) ^ (points[3] - points[0]))[2];
        double p3 = ((points[0] - points[1]) ^ (points[3] - points[1]))[2];
        double p4 = ((points[3] - points[1]) ^ (points[2] - points[1]))[2];
        if (isZero(p1) && isZero(p2)) //Отрезки лежат на одной прямой?
        {
            double dis = Point::distance(points[0], points[2]);
            double dis1 = Point::distance(points[0], points[1]) +
                Point::distance(points[2], points[1]);
            double dis2 = Point::distance(points[0], points[3]) +
                Point::distance(points[2], points[3]);
            if (isZero(dis1 - dis) ||
                isZero(dis2 - dis))
                return true;
            return false;
        }
        else if (equal(p1, p2) || equal(p3, p4)) return false;
        return true;
    }
}

void task1_3()
{
    const char* in_file = "task1_3in.txt";
    std::ifstream in(in_file);

    if (!in)
    {
        std::cerr << "File " << in_file << " not exist or not accessible\n";
    }
    else
    {
        std::vector<Point> points = read(in);
        std::cout << t1_3::isIntersection(points) << std::endl;
    }
    in.close();
}

namespace t1_4
{
    bool equal(int& _sign, int temp)
    {
        if (temp != 0)
        {
            if (_sign == 0) _sign = temp;
            else if (_sign != temp) return false;
        }
        return true;
    }

    bool isConvex(std::vector<Point> points)
    {
        int s = points.size();

        int _sign = sign(((points[s - 1] - points[s - 2]) ^
            (points[0] - points[s - 2]))[2]);
        int temp = sign(((points[0] - points[s - 1]) ^
            (points[1] - points[s - 1]))[2]);
        if (!equal(_sign, temp)) return false;

        for (int i = 0; i < s - 2; i++)
        {
            temp = sign(((points[i + 1] - points[i]) ^
                (points[i + 2] - points[i]))[2]);
            if (!equal(_sign, temp)) return false;
        }
        return true;
    }
}

void task1_4()
{
    const char* in_file = "task1_3in.txt";
    std::ifstream in(in_file);

    if (!in)
    {
        std::cerr << "File " << in_file << " not exist or not accessible\n";
    }
    else
    {
        std::vector<Point> points = read(in);
        std::cout << t1_4::isConvex(points) << std::endl;
    }
    in.close();
}