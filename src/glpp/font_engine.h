#ifndef GLPP_FONT_ENGINE_H
#define GLPP_FONT_ENGINE_H

#include <vector>
#include <string>
#include <filesystem>

#include "font_atlas.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glp {

    class font_engine final {
    public:
        font_engine(const font_engine&) = delete;
        font_engine(const font_engine&&) = delete;
        static font_engine& instance();
        
        bool load(const std::filesystem::path& f);
    private:
        font_engine();

        bool initialised = false;
        bool bad_init = false;
        FT_Library ft;
        font_atlas atlas;
    };

}

#endif
