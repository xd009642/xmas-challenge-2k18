#include "font_atlas.h"
#include <iostream>
#include <algorithm>



glp::font_atlas::font_atlas(FT_Face face, size_t size) {
    if(FT_Set_Pixel_Sizes(face, 0, size)) {
        std::cout<<"Invalid font size selected: "<<size<<std::endl;
    }
    FT_GlyphSlot glyph = face->glyph;
    loaded_chars.reserve(MAX_CHAR-MIN_CHAR+1);
    
    for(char i = MIN_CHAR; i<=MAX_CHAR; i++) {
        if(FT_Load_Char(face, static_cast<FT_ULong>(i), FT_LOAD_RENDER)) {
            continue; // Failed to load the character
        }
        loaded_chars.push_back(i);
        block_width = std::max(block_width, glyph->bitmap.width);
        block_height = std::max(block_height, glyph->bitmap.rows);
    }
}

glp::font_atlas::~font_atlas() {

}


bool glp::font_atlas::font_atlas::valid() const {
    return is_valid;
}
