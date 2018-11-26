#ifndef SYMBOLS_MESH_UTIL_H
#define SYMBOLS_MESH_UTIL_H

#include "../glpp/gl_includes.h"
#include "../glpp/types.h"
#include <vector>

namespace sym {

// Gets normal from line, To rotate 180 degrees negate
template <typename T>
constexpr glp::point<T> get_normal(const glp::line<T> &line) {
    return get_normal(line.first, line.second);
}

// Gets normal from line, To rotate 180 degrees negate
template <typename T>
constexpr glp::point<T> get_normal(const glp::point<T> a,
                                   const glp::point<T> b) {
    glp::point<T> result;
    T dx = b.x - a.x;
    T dy = b.y - a.y;
    result.x = -dy;
    result.y = dx;
    return result.normalise();
}

// Create a mesh for a given line. Returns xy coordinates
// akin to GL_LINESTRIP for now
std::vector<glp::point<GLfloat>>
    make_line_mesh(const std::vector<glp::point<GLfloat>> &points,
                   GLfloat thickness);

std::vector<glp::point<GLfloat>>
    make_scale_line(const glp::point<GLfloat> &left,
                    const glp::point<GLfloat> &right, GLfloat spacing,
                    GLfloat width, GLfloat thickness);
} // namespace sym

#endif
