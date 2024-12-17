#ifndef CIRCLE_H
#define CIRCLE_H

struct Circle
{
    int x;
    int y;
    int width;
    int height;

    Circle(int _x, int _y, int _width, int _height)
        : x(_x), y(_y), width(_width), height(_height) {}
};

#endif // CIRCLE_H
