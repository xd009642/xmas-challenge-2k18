#include "font_engine.h"
#include <iostream>



glp::font_engine& glp::font_engine::instance() {
    static glp::font_engine inst;
    return inst;
}

glp::font_engine::font_engine() {
    if(!initialised && !bad_init) {
        if(FT_Init_FreeType(&ft)) {
            std::cout<<"Failed to initialise freetype"<<std::endl;
            bad_init = true;
        } else {
            std::cout<<"Initialised font_engine"<<std::endl;
            initialised = true;
        }
    }
}


bool glp::font_engine::load(const std::filesystem::path& f) {
    FT_Face face;
    if(FT_New_Face(ft, f.c_str(), 0, &face)) {
        return false;
    }
    // TODO sort out font default sizing
    atlas = glp::font_atlas(face, 16);
    return atlas.valid();
}


glp::font_atlas& glp::font_engine::current_font() {
    return atlas;
}
