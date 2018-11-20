#include "font_atlas.h"
#include <iostream>
#include <algorithm>
#include <cmath>


constexpr uint32_t MAX_WIDTH = 512;
constexpr uint32_t MARGIN_WIDTH = 1;

// See how many fit in row then inc row count. Add padding
// Work out texture size then add them to texture

glp::font_atlas::font_atlas(FT_Face face, size_t size) {
    if(FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        std::cout<<"Font must support unicode"<<std::endl;
    }

    if(FT_Set_Pixel_Sizes(face, 0, size)) {
        std::cout<<"Invalid font size selected: "<<size<<std::endl;
    }
    FT_GlyphSlot glyph = face->glyph;

    auto height = get_atlas_height(face);

    //glGenTextures(1, &tex_id);
    tex = std::make_shared<glp::texture>();
    if(tex) {
        tex_id = tex->id();
    }
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    std::vector<uint8_t> init(MAX_WIDTH * height, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, MAX_WIDTH, 
            height, 0, GL_RED, GL_UNSIGNED_BYTE, init.data());
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    size_t w = MARGIN_WIDTH;
    size_t h = MARGIN_WIDTH;
    uint32_t max_height=0;
    for(char i = MIN_CHAR; i<=MAX_CHAR; i++) {
        if(FT_Load_Char(face, static_cast<FT_ULong>(i), FT_LOAD_RENDER)) {
            continue;
        }
        if(w + glyph->bitmap.width > MAX_WIDTH) {
            w = MARGIN_WIDTH;
            h += max_height;
            max_height = 0;
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, w, h, glyph->bitmap.width, 
                glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
        
        max_height = std::max(max_height, glyph->bitmap.rows);

        glp::entry in;
        in.width = glyph->bitmap.width;
        in.height = glyph->bitmap.rows;
        in.tx0 = static_cast<GLfloat>(w)/static_cast<GLfloat>(MAX_WIDTH);
        in.tx1 = static_cast<GLfloat>(w + in.width)/static_cast<GLfloat>(MAX_WIDTH);
        in.ty0 = static_cast<GLfloat>(h + in.height + MARGIN_WIDTH)/static_cast<GLfloat>(height);
        in.ty1 = static_cast<GLfloat>(h)/static_cast<GLfloat>(height);
        in.x_increment = glyph->advance.x/64.0f;
        in.y_increment = glyph->advance.y/64.0f;
        in.left = glyph->bitmap_left;
        in.top = glyph->bitmap_top;
        characters[i] = in;
        w += glyph->bitmap.width + MARGIN_WIDTH;
    }

    if(!FT_Load_Char(face, ' ', FT_LOAD_RENDER)) {
        space = glyph->advance.x/(size*4); 
    } else {
        space = MARGIN_WIDTH * 5;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    dim.length[0] = MAX_WIDTH;
    dim.length[1] = height;
    is_valid = true;
}

glp::font_atlas::~font_atlas() {
    // No need to destroy texture shared_ptr<texture> does that
}


uint32_t glp::font_atlas::get_atlas_height(FT_Face& face) {
    uint32_t result = MARGIN_WIDTH * 2;
    FT_GlyphSlot glyph = face->glyph;

    uint32_t current_width = MARGIN_WIDTH * 2;
    uint32_t row_height = 0;
    
    for(char i = MIN_CHAR; i<=MAX_CHAR; i++) {
        if(FT_Load_Char(face, static_cast<FT_ULong>(i), FT_LOAD_RENDER)) {
            continue; // Failed to load the character
        }
        current_width += glyph->bitmap.width + MARGIN_WIDTH;
        if(current_width > MAX_WIDTH) {
            result += row_height + MARGIN_WIDTH;
            row_height = 0;
            current_width = MARGIN_WIDTH * 2;
        } else {
            row_height = std::max(glyph->bitmap.rows, row_height);
        }
    }
    result += row_height;
    return result;
}

bool glp::font_atlas::font_atlas::valid() const {
    return is_valid;
}

glp::generic_lock glp::font_atlas::font_atlas::bind() {
    glBindTexture(GL_TEXTURE_2D, tex_id);
    return glp::generic_lock([](){glBindTexture(GL_TEXTURE_2D, 0);});
}


bool glp::font_atlas::contains(const char c) const {
    return characters.find(c) != characters.end();
}

const glp::entry& glp::font_atlas::get_entry(const char c) const {
    if(!contains(c)) {
        throw std::out_of_range("Character not in atlas");
    }
    return characters.find(c)->second;
}

        
glp::range2u glp::font_atlas::dimensions() const {
    return dim;
}


float glp::font_atlas::space_width() const { 
    return space;
}
