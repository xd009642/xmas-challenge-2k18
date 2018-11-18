#ifndef GLPP_TYPES_H
#define GLPP_TYPES_H

#include <array>

namespace glp {

    template<typename T, size_t dim>
    struct range {
        std::array<T, dim> length;
    };

    using range2u = range<uint32_t, 2>;
    
    template<typename T>
    struct point {
        T x;
        T y;
    };

    template<typename T>
    struct rect {
        point<T> top_left;
        point<T> bottom_right;
    };
   
    struct mesh_2d {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    };
    
}

#endif
