#ifndef SYMBOLS_MESH_UTIL_H
#define SYMBOLS_MESH_UTIL_H

#include <vector>
#include "../glpp/gl_includes.h"
#include "../glpp/types.h"

namespace sym {

    
    template<typename T>
    glp::line<T> get_normal(const glp::line<T>& line) {
        return {};
    }

    // Create a mesh for a given line. Returns xy coordinates
    // akin to GL_LINESTRIP for now
    std::vector<glp::point<GLfloat>> make_line_mesh(const std::vector<glp::point<GLfloat>>& points, 
            GLfloat thickness);

}


#endif
