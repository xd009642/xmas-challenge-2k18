#include "mesh_util.h"
#include <algorithm>

const GLfloat MIN_THICKNESS = 0.01f;


std::vector<glp::point<GLfloat>> sym::make_line_mesh(
        const std::vector<glp::point<GLfloat>>& points, 
        GLfloat thickness) {
    thickness = std::min(thickness, MIN_THICKNESS);
    const double radius = thickness/2;

    return {};
}
