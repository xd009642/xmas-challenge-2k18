#ifndef GLPP_TYPES_H
#define GLPP_TYPES_H

namespace glp {
    
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
    
    
}

#endif
