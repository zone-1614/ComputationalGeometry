#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include <vector>
#include <algorithm>

using Kernel = CGAL::Simple_cartesian<double>;
using Point_2 = Kernel::Point_2;
using Segment_2 = Kernel::Segment_2;
using Points = std::vector<Point_2>;
using Segments = std::vector<Segment_2>;

int main() {
  // init the point set 
  Points ps;
  ps.push_back(Point_2(1, 2));
  ps.push_back(Point_2(1, 9));
  ps.push_back(Point_2(9, 2));
  ps.push_back(Point_2(9, 8));
  ps.push_back(Point_2(4, 5));
  ps.push_back(Point_2(6, 4));
  Segments ss;

  // for each segment
  for (int i = 0; i < ps.size(); i++) {
    Point_2 p = ps[i];
    for (int j = 0; j < ps.size(); j++) {
      if (j == i) continue;
      Point_2 q = ps[j];
      Segment_2 pq(p, q);
      bool valid = true;
      for (int k = 0; k < ps.size(); k++) {
        if (k == j || k == i) continue;
        Point_2 r = ps[k];
        // std::cout << p << " " << q << " " << r << " " << CGAL::orientation(p, q, r) << std::endl;
        // all points should lie in the left side of segment pq.
        if (CGAL::orientation(p, q, r) != CGAL::LEFT_TURN) {
          valid = false;
        }
      }
      if (valid) {
        ss.push_back(pq);
      }
    }
  }

  Segments convex_hull;
  convex_hull.push_back(ss.back());
  ss.pop_back();
  while (!ss.empty()) {
    auto start = convex_hull.back().end();
    auto segment = std::find_if(ss.begin(), ss.end(), [&start](auto s) { return s.start() == start; });
    convex_hull.push_back(*segment);
    ss.erase(segment);
  }
  for (int i = 0; i < convex_hull.size(); i++) {
    std::cout << convex_hull[i] << std::endl;
  }
}