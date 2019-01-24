#ifndef GLPP_TYPES_H
#define GLPP_TYPES_H

#include <array>
#include <cmath>
#include <glm/glm.hpp>
#include <type_traits>
#include <utility>

namespace glp {

template <typename T, size_t dim> struct range { std::array<T, dim> length; };

using range2u = range<uint32_t, 2>;

template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>>
struct point {
    point(T x, T y) : x(x), y(y) {
    }
    point() {
    }

    T x;
    T y;

    point<T> &operator+=(const point<T> &i) {
        x += i.x;
        y += i.y;
        return *this;
    }

    point<T> &operator-=(const point<T> &i) {
        x -= i.x;
        y -= i.y;
        return *this;
    }

    point<T> &operator*=(const T &i) {
        x *= i;
        y *= i;
        return *this;
    }

    point<T> normalise() const {
        const T scale = magnitude();
        return point<T>(x / scale, y / scale);
    }

    glm::vec2 as_glm_type() const {
        return glm::vec2(x, y);
    }

    T magnitude() const {
        return std::sqrt(std::pow(x, static_cast<T>(2)) +
                         std::pow(y, static_cast<T>(2)));
    }
};

template <typename T>
inline point<T> operator+(point<T> lhs, const point<T> &rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
inline point<T> operator-(point<T> lhs, const point<T> &rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T> inline point<T> operator*(point<T> lhs, const T &rhs) {
    lhs *= rhs;
    return lhs;
}

template <typename T> inline point<T> operator*(const T &lhs, point<T> rhs) {
    rhs *= lhs;
    return rhs;
}

template <typename T> using line = std::pair<point<T>, point<T>>;

template <typename T> struct rect {
    point<T> top_left;
    point<T> bottom_right;
};

struct mesh_2d {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};

} // namespace glp

#endif
