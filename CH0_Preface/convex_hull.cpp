#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <vector>

// using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
// using Point_2 = Kernel::Point_2;
// using Points = std::vector<Point_2>;
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef std::vector<Point_2> Points;

int main() {
  // built-in array convex hull
  Point_2 points[5] = { Point_2(0,0), Point_2(10,0), Point_2(10,10), Point_2(6,5), Point_2(4,1) };
  Point_2 result[5];

  Point_2 *ptr = CGAL::convex_hull_2( points, points + 5, result );
  std::cout << ptr - result << " points on the convex hull" << std::endl;
  for (int i = 0; i < ptr - result; i++) {
    std::cout << result[i] << std::endl;
  }

  // vector convex hull
  Points ps, re;
  ps.push_back(Point_2(0,0));
  ps.push_back(Point_2(10,0));
  ps.push_back(Point_2(10,10));
  ps.push_back(Point_2(6,5));
  ps.push_back(Point_2(4,1));

  CGAL::convex_hull_2(ps.begin(), ps.end(), std::back_inserter(re));
  std::cout << re.size() << " points on the convex hull" << std::endl;
  for (int i = 0; i < re.size(); i++) {
    std::cout << re[i] << std::endl;
  }
  return 0;
}