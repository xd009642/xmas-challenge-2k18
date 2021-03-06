#include "font_engine.h"
#include <iostream>

glp::font_engine &glp::font_engine::instance() {
    static glp::font_engine inst;
    return inst;
}

glp::font_engine::font_engine() {
    if(!initialised && !bad_init) {
        if(FT_Init_FreeType(&ft)) {
            std::cout << "Failed to initialise freetype" << std::endl;
            bad_init = true;
        } else {
            std::cout << "Initialised font_engine" << std::endl;
            initialised = true;
        }
    }
}

glp::font_engine::~font_engine() {
    FT_Done_FreeType(ft);
}

void glp::font_engine::set_program(const glp::program &prog) {
    default_shader = prog;
}

bool glp::font_engine::load(const std::filesystem::path &f) {
    FT_Face face;
    if(FT_New_Face(ft, f.c_str(), 0, &face)) {
        return false;
    }
    // TODO sort out font default sizing
    atlas = glp::font_atlas(face, 32);
    FT_Done_Face(face);
    return atlas.valid();
}

glp::font_atlas &glp::font_engine::current_font() {
    return atlas;
}

void glp::font_engine::render_text(const std::string_view text, float x,
                                   float y, float sx, float sy, glp::colour c) {

    if(text.empty()) {
        return;
    }

    if(!vbo) {
        vbo = std::make_unique<glp::buffer_obj>();
    }

    if(!default_shader.valid()) {
        std::cout << "Don't have a valid shader" << std::endl;
    }

    GLint colour = default_shader.get_uniform("colour");
    GLint tex_i = default_shader.get_uniform("texture");
    GLint coord = default_shader.get_attrib("vert");
    if(colour == -1 || tex_i == -1) {
        std::cout << "Failed to find one or more uniforms" << std::endl;
    }
    if(vbo) {

        float height = glutGet(GLUT_WINDOW_HEIGHT);
        float width = glutGet(GLUT_WINDOW_WIDTH);
        default_shader.attach();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUniform4fv(colour, 1, c.data());
        auto tex_lock = atlas.bind();
        glUniform1i(tex_i, 0);

        glEnableVertexAttribArray(coord);
        glBindBuffer(GL_ARRAY_BUFFER, vbo->id());
        glVertexAttribPointer(coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

        std::vector<glp::mesh_2d> coords = get_text_mesh(text, x, y, sx, sy);
        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(glp::mesh_2d),
                     coords.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, coords.size());
        glDisableVertexAttribArray(coord);
    }
}

std::vector<glp::mesh_2d> glp::font_engine::get_text_mesh(const std::string_view text, float x,
                                    float y, float sx, float sy) {
    
    float height = glutGet(GLUT_WINDOW_HEIGHT);
    float width = glutGet(GLUT_WINDOW_WIDTH);
    std::vector<glp::mesh_2d> coords;
    float max_height = 0;
    float start_x = x;
    // Sort out coordinates and texturing
    for(const char &c : text) {
        if(atlas.contains(c)) {
            const auto &e = atlas.get_entry(c);
            size_t start = coords.size();
            coords.resize(start + 6);
            const float y_temp = y - (e.height - e.top) * sy;
            const float x_temp = x + e.left * sx;
            const float h_temp = e.height * sy;
            const float w_temp = e.width * sx;
            // Set the 6 coordinates
            coords[start].x = (x_temp) / width;
            coords[start].y = y_temp / height;
            coords[start].s = e.tx0;
            coords[start++].t = e.ty0;

            coords[start].x = (x_temp) / width;
            coords[start].y = (y_temp + h_temp) / height;
            coords[start].s = e.tx0;
            coords[start++].t = e.ty1;

            coords[start].x = (x_temp + w_temp) / width;
            coords[start].y = (y_temp + h_temp) / height;
            coords[start].s = e.tx1;
            coords[start++].t = e.ty1;

            coords[start].x = (x_temp) / width;
            coords[start].y = (y_temp) / height;
            coords[start].s = e.tx0;
            coords[start++].t = e.ty0;

            coords[start].x = (x_temp + w_temp) / width;
            coords[start].y = (y_temp + h_temp) / height;
            coords[start].s = e.tx1;
            coords[start++].t = e.ty1;

            coords[start].x = (x_temp + w_temp) / width;
            coords[start].y = (y_temp) / height;
            coords[start].s = e.tx1;
            coords[start].t = e.ty0;

            x += e.x_increment * sx;
            y += e.y_increment * sy;
            max_height = std::max(max_height, h_temp);
        } else { // 0 width char like space or \n \r
            // advance x and y
            x += 10;
            if(c == '\n' || c == '\r') {
                x = start_x;
                y -= max_height + 5;
            } else if(c == ' ') {
                x += atlas.space_width();
            }
        }
    }
    return coords;
}


glp::point<float> glp::font_engine::text_dimensions(const std::string_view text) const {
    float height = glutGet(GLUT_WINDOW_HEIGHT);
    float width = glutGet(GLUT_WINDOW_WIDTH);
    float x = 0.0f, y = 0.0f;

    float max_height = 0.0f, max_width= 0.0f;
    
    for(const char &c : text) {
        if(atlas.contains(c)) {
            const auto &e = atlas.get_entry(c);
            x += e.x_increment;
            y = std::max(y, static_cast<float>(e.height));
        } else { // 0 width char like space or \n \r
            // advance x and y
            x += 10;
            if(c == '\n' || c == '\r') {
                max_width = std::max(max_width, x);
                max_height += y;
                y = 0.0f;
                x = 0.0f;
            } else if(c == ' ') {
                x += atlas.space_width();
            }
        }
    }
    max_width = std::max(max_width, x);
    if(text.back() != '\n' && text.back() != '\r') {
        max_height += y;
    }
    return glp::point<float>(max_width, max_height);
}
