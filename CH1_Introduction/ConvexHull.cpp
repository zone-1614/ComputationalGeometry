// compute the convex hull.
#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include <vector>
#include <algorithm>

using Kernel = CGAL::Simple_cartesian<double>;
using Point_2 = Kernel::Point_2;
using Points = std::vector<Point_2>;

bool LastThreePointMakeRightTurn(const Points& points);

int main() {
  // init, input: a set of points
  Points ps;
  ps.push_back(Point_2(1, 2));
  ps.push_back(Point_2(9, 2));
  ps.push_back(Point_2(9, 8));
  ps.push_back(Point_2(4, 5));
  ps.push_back(Point_2(6, 5));
  ps.push_back(Point_2(7, 3));
  ps.push_back(Point_2(5, 5));
  ps.push_back(Point_2(6, 4));
  ps.push_back(Point_2(3, 10));
  ps.push_back(Point_2(5, 15));
  ps.push_back(Point_2(7, 9));
  ps.push_back(Point_2(6, -9));

  // output: a list contain the vertices of ConvexHull
  Points output;
  Points upper_hull, lower_hull;

  // begin the algorithm
  // 1. sort the input by x-coordinate
  std::sort(ps.begin(), ps.end(), [](Point_2& l, Point_2& r) { return l.x() < r.x(); });
  // std::for_each(ps.begin(), ps.end(), [](Point_2& p) {std::cout << p << std::endl;});
  // 2. put p_1 p_2 in list
  upper_hull.push_back(ps[0]); 
  upper_hull.push_back(ps[1]); 
  // 3. upper hull
  int n = ps.size();
  for (int i = 2; i < n; i++) {
    upper_hull.push_back(ps[i]);
    while (
      upper_hull.size() >= 2 // contain more than 2 points
      && !LastThreePointMakeRightTurn(upper_hull) // the last three points do not make a right turn
    ) {
      // delete the middle of last three points
      int n = upper_hull.size();
      upper_hull[n-2] = upper_hull[n-1];
      upper_hull.pop_back();
    }
  }
  // 4. lower hull
  // put p_n, p_{n-1} in lower hull
  lower_hull.push_back(ps[n - 1]);
  lower_hull.push_back(ps[n - 2]);
  for (int i = n - 3; i >= 0; i--) {
    // append ps[i] to lower hull
    lower_hull.push_back(ps[i]);
    while (
      lower_hull.size() >= 2 // contain more than 2 points
      && !LastThreePointMakeRightTurn(lower_hull) // the last three points do not make a right turn
    ) {
      // delete the middle of last three points
      int n = lower_hull.size();
      lower_hull[n-2] = lower_hull[n-1];
      lower_hull.pop_back();
    }
  }
  // 5. append upper hull and lower hull to the output, except the first and last element in lower hull to avoid duplication. 
  output.insert(output.end(), upper_hull.begin(), upper_hull.end());
  output.insert(output.end(), lower_hull.begin() + 1, lower_hull.end() - 1);
  std::for_each(output.begin(), output.end(), [](Point_2& p) {std::cout << p << std::endl;});

  // output:
  // 1 2
  // 3 10
  // 5 15
  // 9 8
  // 9 2
  // 6 -9
}

bool LastThreePointMakeRightTurn(const Points& points) {
  int n = points.size() - 1;
  Point_2 p = points[n - 2], q = points[n - 1], r = points[n];
  return (CGAL::orientation(p, r, q) == CGAL::Sign::LEFT_TURN);
}