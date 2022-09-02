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

Result FindIntersections(Segments&);
void HandleEventPoint(Point&, Segments& );
void FindNewEvent(const Segment&, const Segment&);
double x_by_sweep_line(const Segment& s);

// event point compare function. 
auto compareEventPoint = [](const Point& p1, const Point& p2) -> bool {
  if (p1.y() != p2.y()) {
    return p1.y() < p2.y();
  } else {
    return p1.x() < p2.x();
  }
};

// get the x-axis by sweep line. 
double x_by_sweep_line(const Segment& s) {
  if (s.is_vertical()) {
    return s.start().x();
  }
  auto p1 = s.start(), p2 = s.end();
  double slope = (p1.y() - p2.y()) / (p1.x() - p2.x());
  double y = p1.x() + (sweep_line - p1.y()) / slope;
  return y;
}

// status compare function.
auto compareStatusNode = [](const Segment& s1, const Segment& s2) -> bool {
  return x_by_sweep_line(s1) < x_by_sweep_line(s2);
};

Status status(compareStatusNode);

Result result;

// event queue stores the events. 
EventQueue q(compareEventPoint);

int main() {
  
}

Result FindIntersections(Segments& segments) {
  // insert all endpoints into q. 
  for (Segment& sg : segments) {
    q.push(sg.start());
    q.push(sg.end());
  }
  
  while (!q.empty()) {
    // determine the next event point p and delete it
    Point p = q.top();
    sweep_line = p.y();
    q.pop();

    HandleEventPoint(p, segments);
  }
}

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
    // find an intersection point.
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
    
  if (U.size() + I.size() == 0) {
    // find left and right neighbour. 
    Segment sl, sr;
    for (auto it = status.begin(); it != status.end(); it++) {
      if ((*it).has_on(p)) {
        auto o = it++;
        if (it != status.end()) {
          sr = *it;
        }
        if (o != status.begin()) {
          o--;
          sl = *o;
        }
      }
    }
    FindNewEvent(sl, sr);
  } else {
    Segments U_cup_C;
    U_cup_C.insert(U_cup_C.begin(), U.begin(), U.end());
    U_cup_C.insert(U_cup_C.begin(), I.begin(), I.end());

    Segment sl, sll;
    Segment sr, srr;
    bool left = false, right = false; // need to find new event on left/right side. 
    for (auto it = status.begin(); it != status.end(); it++) {
      if (std::find(U_cup_C.begin(), U_cup_C.end(), *it) != U_cup_C.end()) {
        sl = *it;
        if (it != status.begin()) {
          sll = *(--it);
          left = true;
        }
      }
    }
    for (auto rit = status.rbegin(); rit != status.rend(); rit++) {
      if (std::find(U_cup_C.begin(), U_cup_C.end(), *rit) != U_cup_C.end()) {
        sr = *rit;
        if (rit != status.rbegin()) {
          srr = *(--rit);
          right = true;
        }
      }
    }
    if (left)
      FindNewEvent(sll, sl);
    if (right)
      FindNewEvent(sr, srr);
  }
}

void FindNewEvent(const Segment& s1, const Segment& s2) {
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
    auto v = ans.value();
    if (/* if v not in q */)
      q.push(v);
  }
}
