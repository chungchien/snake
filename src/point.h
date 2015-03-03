#ifndef SNAKE_SRC_POINT_H_
#define SNAKE_SRC_POINT_H_

struct Point {
  int x;
  int y;
};

#define point_equal(pt1, pt2) ((pt1).x == (pt2).x && (pt1).y == (pt2).y)

#endif // SNAKE_SRC_POINT_H_
