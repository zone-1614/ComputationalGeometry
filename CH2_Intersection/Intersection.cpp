#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <vector>
#include <functional>
#include <map>

#include <CGAL/Simple_cartesian.h>

using Kernel = CGAL::Simple_cartesian<double>; 
using Point = Kernel::Point_2;
using Segment = Kernel::Segment_2;
using Segments = std::vector<Segment>;
using EventQueue = std::priority_queue<Point, std::vector<Point>, std::function<bool(const Point&, const Point&)>>;

using Status = std::multiset<Segment, std::function<bool(const Segment&, const Segment&)>>;
using Result = std::map<Point, Segments>;

double sweep_line;

void FindIntersections(Segments&);
void HandleEventPoint(Point&, Segments& );
void FindNewIntersectionPoint(const Segment&, const Segment&, const Point&);

// event point compare function. 
auto compareEventPoint = [](const Point& p1, const Point& p2) -> bool {
  if (p1.y() != p2.y()) {
    return p1.y() < p2.y();
  } else {
    return p1.x() < p2.x();
  }
};

// get the x-axis by sweep line. 
double x_by_sweep_line(const Segment& s);

// status compare function.
auto compareStatusNode = [](const Segment& s1, const Segment& s2) -> bool {
  return x_by_sweep_line(s1) < x_by_sweep_line(s2);
};

Status status(compareStatusNode);

Result result;

// event queue stores the events. 
EventQueue q(compareEventPoint);
std::vector<Point> __q; // only use for detect whether a point in q. 

int main() {
  Segments segs;
  segs.push_back(Segment(Point(4, 4), Point(1, 1)));
  segs.push_back(Segment(Point(1, 3), Point(4, 0)));
  segs.push_back(Segment(Point(2, 2.5), Point(3, 1.5)));
  segs.push_back(Segment(Point(3, 2), Point(2, 1)));
  segs.push_back(Segment(Point(1.5, 8), Point(2, 0)));

  FindIntersections(segs);
  
  for (auto& entry : result) {
    std::cout << "point: " << entry.first << ", size of segs: " << entry.second.size() << std::endl;
  }
}

/**
 * @brief Get x-axis by the global variable sweep_line
 * @param s The segment which need to get x-axis
 * @return double
 */
double x_by_sweep_line(const Segment& s) {
  if (s.is_vertical()) {
    return s.start().x();
  }
  auto p1 = s.start(), p2 = s.end();
  double slope = (p1.y() - p2.y()) / (p1.x() - p2.x());
  double y = p1.x() + (sweep_line - p1.y()) / slope;
  return y;
}

/**
 * @brief Find all intersection points in the input segment set. 
 * @param segments The segment set. 
 */
void FindIntersections(Segments& segments) {
  // insert all endpoints into q. 
  for (Segment& sg : segments) {
    q.push(sg.start());
    q.push(sg.end());
    __q.push_back(sg.start());
    __q.push_back(sg.end());
  }
  
  while (!q.empty()) {
    // determine the next event point p and delete it
    Point p = q.top();
    std::remove(__q.begin(), __q.end(), p);
    __q.resize(__q.size() - 1);
    sweep_line = p.y();
    q.pop();

    HandleEventPoint(p, segments);
  }
}

/**
 * @brief Determine whether the event point an intersection point and find the related event point.
 * 
 * @param p The event point. 
 * @param segments The segment set. 
 */
void HandleEventPoint(Point& p, Segments& segments) {
  Segments U, L, I;
  for (Segment& sg : segments) {
    if (sg.start() == p)
      U.push_back(sg);
  }

  // push all segments which lower endpoint is p into L
  // push all segments internally contain p into I
  for (auto& entry : status) {
    if (entry.end() == p) {
      L.push_back(entry);
    } else if (entry.start() != p && entry.has_on(p)) {
      I.push_back(entry);
    }
  }

  if (U.size() + L.size() + I.size() > 1) {
    // find an intersection point. Store the intersection point and all intersection segments together in result. 
    Segments s;
    s.insert(s.end(), U.begin(), U.end());
    s.insert(s.end(), L.begin(), L.end());
    s.insert(s.end(), I.begin(), I.end());
    result[p] = s;
  }
  // delete segments in L or I
  for (Segment& seg : L) 
    status.erase(seg);
  for (Segment& seg : I)
    status.erase(seg);

  // insert segments in U
  for (Segment& seg : U)
    status.insert(seg);
  
  // inverted insert segments in I
  for (auto rit = I.rbegin(); rit != I.rend(); rit++) 
    status.insert(*rit);
    
  if (U.size() + I.size() == 0) { // event point is the lower endpoint of a segment. 
    // find left and right neighbour. 
    Segment sl, sr;
    bool find_left = false, find_right = false;
    for (auto it = status.begin(); it != status.end(); it++) {
      if ((*it).has_on(p)) {
        auto o = it++;
        if (it != status.end()) {
          sr = *it;
          find_right = true;
        }
        if (o != status.begin()) {
          o--;
          sl = *o;
          find_left = true;
        }
      }
    }
    if (find_left && find_right)
      FindNewIntersectionPoint(sl, sr, p);
  } else { // event point is a intersection point. 
    Segments U_cup_C;
    U_cup_C.insert(U_cup_C.begin(), U.begin(), U.end());
    U_cup_C.insert(U_cup_C.begin(), I.begin(), I.end());

    Segment sl, sll;
    Segment sr, srr;
    bool left = false, right = false; // need to find new event on left/right side. 
    // In status, find first segment in U cup C (assigned sl), and find the left neighbour of sl (assigned sll). 
    for (auto it = status.begin(); it != status.end(); it++) {
      if (std::find(U_cup_C.begin(), U_cup_C.end(), *it) != U_cup_C.end()) {
        sl = *it;
        if (it != status.begin()) {
          sll = *(--it);
          left = true;
          break;
        }
      }
    }
    // In status, find first segment in U cup C (assigned sr), and find the right neighbour of sr (assigned srr). 
    for (auto rit = status.rbegin(); rit != status.rend(); rit++) {
      if (std::find(U_cup_C.begin(), U_cup_C.end(), *rit) != U_cup_C.end()) {
        sr = *rit;
        if (rit != status.rbegin()) {
          srr = *(--rit);
          right = true;
          break;
        }
      }
    }
    if (left)
      FindNewIntersectionPoint(sll, sl, p);
    if (right)
      FindNewIntersectionPoint(sr, srr, p);
  }
}

/**
 * @brief Find the intersection point by the input segments, and determine whether push it into q by the current event point p. 
 * 
 * @param s1 segment 1
 * @param s2 segment 2
 * @param p the current event point. 
 */
void FindNewIntersectionPoint(const Segment& s1, const Segment& s2, const Point& p) {
  // intersection point between two segment. 
  auto intersect = [&]() -> std::optional<Point> {
    auto i = CGAL::intersection(s1, s2);
    if (i.has_value()) {
      return boost::get<Point>(i.value());
    } else {
      return std::nullopt;
    }
  };
  auto ans = intersect();

  if (ans.has_value()) {
    auto v = ans.value(); // v is the intersection point. 
    
    if (/* if v not in q */ std::find(__q.begin(), __q.end(), v) == __q.end()
    && /* v is lower than p */ v.y() < p.y()) {
      q.push(v);
      __q.push_back(v);      
    }
  }
}
