#ifndef SNAKE_SRC_WALL_H_
#define SNAKE_SRC_WALL_H_

struct Point;
struct Wall;

struct Wall *wall_init(int top, int left, int bottom, int right);
void wall_delete(struct Wall *wall);
/* 如果點point在墻外返回非零值，否則返回零 */
int wall_is_out_range(const struct Wall *wall, const struct Point *point);
void wall_paint(const struct Wall *wall);

#endif // SNAKE_SRC_WALL_H_
