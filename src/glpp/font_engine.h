#ifndef GLPP_FONT_ENGINE_H
#define GLPP_FONT_ENGINE_H

#include <vector>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glp {

    class font_engine final {
    public: 
        font_engine();
    private:
        static bool initialised;
        static bool bad_init;
        static FT_Library ft;
    };

}

#endif
