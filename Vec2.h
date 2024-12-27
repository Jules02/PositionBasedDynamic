#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

struct Vec2
{
    float components[2] = { 0.0, 0.0 };

    float operator[](int i) const { return components[i]; }
};

inline float dot(const Vec2& v0, const Vec2& v1)
{
    return v0.components[0] * v1.components[0]
           + v0.components[1] * v1.components[1];
}

inline float squaredLength(const Vec2& v)
{
    return v.components[0] * v.components[0]
           + v.components[1] * v.components[1];
}

inline float length(const Vec2& p)
{
    return std::sqrt(squaredLength(p));
}


inline Vec2 operator-(const Vec2 &v) { return {-v.components[0], -v.components[1] }; }
inline Vec2 operator+(const Vec2& v0, const Vec2& v1) {
    return {{ v0.components[0] + v1.components[0], v0.components[1] + v1.components[1] }};
}
inline Vec2 operator-(const Vec2& v0, const Vec2& v1) {
    return {{ v0.components[0] - v1.components[0], v0.components[1] - v1.components[1]}};
}
inline Vec2 operator*(float t, const Vec2& v) {
    return {{ t*v.components[0], t*v.components[1] }};
}

inline Vec2 normalize(const Vec2& p)
{
    return 1/length(p) * p;
}

inline Vec2 perpendicular(const Vec2& p)
{
    return normalize({{ -p.components[1], p.components[0] }});
}

inline std::ostream& operator<<(std::ostream& out, const Vec2& v)
{
    return out << v.components[0] << " " << v.components[1];
}

#endif // VEC2_H
