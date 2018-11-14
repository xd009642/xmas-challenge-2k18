#include "font_atlas.h"
#include <iostream>
#include <algorithm>



glp::font_atlas::font_atlas(FT_Face face, size_t size) {
    if(FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        std::cout<<"Font must support unicode"<<std::endl;
    }

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

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    
    size_t cols = (2+block_width) * loaded_chars.size();
    size_t rows = (2+block_height);

    std::vector<uint8_t> init(rows*cols, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, cols, 
            rows, 0, GL_RED, GL_UNSIGNED_BYTE, init.data());
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    size_t w = 1;
    for(const char& c: loaded_chars) {
        if(FT_Load_Char(face, static_cast<FT_ULong>(c), FT_LOAD_RENDER)) {
            std::cout<<"Failed to reload "<<c<<std::endl;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, w, 0, glyph->bitmap.width, 
                glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
        w += block_width;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    is_valid = true;
}

glp::font_atlas::~font_atlas() {

}


bool glp::font_atlas::font_atlas::valid() const {
    return is_valid;
}


glp::generic_lock glp::font_atlas::font_atlas::bind() {
    glBindTexture(GL_TEXTURE_2D, tex_id);
    return glp::generic_lock([](){glBindTexture(GL_TEXTURE_2D, 0);});
}


bool glp::font_atlas::contains(const char c) const {
    return loaded_chars.find(c) != std::string::npos;
}


glp::rect<double> glp::font_atlas::get_coords(const char c) const { 
    size_t index = loaded_chars.find(c);

    if(index != std::string::npos) {
        glp::rect<double> result;
        glp::point<double> tl;
        glp::point<double> br;

        tl.y = 1.0;
        br.y=0.0;
    
        auto width = static_cast<double>(block_width * loaded_chars.size());
        tl.x = static_cast<double>(index)/width;
        if(index+1 == loaded_chars.size()) {
            br.x = 1.0;
        } else {
            br.x = static_cast<double>(index+1)/width;
        }
        result.top_left = tl;
        result.bottom_right = br;
        
        return result;
    }
    return {{0.0, 0.0}, {0.0, 0.0}};
}
