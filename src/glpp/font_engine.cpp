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


glp::font_engine::~font_engine() {
    FT_Done_FreeType(ft);
}


void glp::font_engine::set_program(const glp::program& prog) {
    default_shader = prog;
}


bool glp::font_engine::load(const std::filesystem::path& f) {
    FT_Face face;
    if(FT_New_Face(ft, f.c_str(), 0, &face)) {
        return false;
    }
    // TODO sort out font default sizing
    atlas = glp::font_atlas(face, 16);
    FT_Done_Face(face);
    return atlas.valid();
}


glp::font_atlas& glp::font_engine::current_font() {
    return atlas;
}


void glp::font_engine::render_text(const std::string_view text, 
        float x,
        float y,
        float sx, 
        float sy) {

    auto vbo = std::make_shared<glp::buffer_obj>();
    if(vbo) {
        default_shader.attach();
        auto tex_lock = atlas.bind();
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo->id());
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

        std::vector<glp::mesh_2d> coords(text.size());
        size_t max_height = 0;
        float start_x = x;
        // Sort out coordinates and texturing
        for(const char& c: text) {
            if(atlas.contains(c)) {
                const auto& e = atlas.get_entry(c);
                size_t start = coords.size();
                coords.resize(start + 6);
                
                // Set the 6 coordinates
                coords[start]. x = x;
                coords[start].y = y;
                coords[start].s = e.tx0;
                coords[start++].t = e.ty0;

                coords[start].x = x;
                coords[start].y = y + e.height;
                coords[start].s = e.tx0;
                coords[start++].t = e.ty1;
                
                coords[start]. x = x + e.width;
                coords[start].y = y + e.height;
                coords[start].s = e.tx1;
                coords[start++].t = e.ty1;
                
                coords[start].x = x;
                coords[start].y = y;
                coords[start].s = e.tx0;
                coords[start++].t = e.ty0;
                
                coords[start]. x = x + e.width;
                coords[start].y = y + e.height;
                coords[start].s = e.tx1;
                coords[start++].t = e.ty1;
                
                coords[start].x = x + e.width;
                coords[start].y = y;
                coords[start].s = e.tx1;
                coords[start].t = e.ty0;
                
                x += e.width;
                max_height = std::max(max_height, e.height);
            } else { // 0 width char like space or \n \r
                // advance x and y
                x += 10;
                if(c == '\n' || c == '\r') {
                    x = start_x;
                    y += max_height;
                    max_height = 0;
                }
            }
        }
        
        glBufferData(GL_ARRAY_BUFFER, coords.size()*sizeof(uint32_t), coords.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, coords.size());
        glDisableVertexAttribArray(0);

        default_shader.detach();
    }
}
