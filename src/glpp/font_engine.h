#ifndef GLPP_FONT_ENGINE_H
#define GLPP_FONT_ENGINE_H

#include "font_atlas.h"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glp {

    class font_engine final {
    public:
        font_engine(const font_engine&) = delete;
        font_engine(const font_engine&&) = delete;
        ~font_engine();
        static font_engine& instance();
        
        font_atlas& current_font();

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
