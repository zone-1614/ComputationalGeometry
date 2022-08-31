#include <iostream>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

int main() {
  {
    Point_2 p(0, 0.3), q(1, 0.6), r(2, 0.9);
    std::cout << (CGAL::collinear(p, q, r) ? "collinear\n" : "not collinear\n"); // not collinear
  }
  {
    Point_2 p(0, 1.0/3.0), q(1, 2.0/3.0), r(2, 1);
    std::cout << (CGAL::collinear(p, q, r) ? "colinear\n" : "not collinear\n"); // not collinear
  }
  {
    Point_2 p(0, 0), q(1, 1), r(2, 2);
    std::cout << (CGAL::collinear(p, q, r) ? "colinear\n" : "not collinear\n"); // collinear
  }
  // we except to get three times "collinear", however only get one.
  // because some numbers are not representable as double-precision numbers, 
  // and the collinear test will compute a determinant of 3x3 matrix
  // which is close but not eq to 0, and hence non collinearity for the first two tests.
  // next we use the exact one in file : exact.cpp
  return 0;
}