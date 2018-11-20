#include "test_patterns.h"
#include <iostream>

static std::string empty_vert = "#version 120\n"
    "attribute vec2 coord;"
    "void main(void) {"
    "   gl_Position = vec4(coord.xy, 0.0, 1.0);"
    "}";

void xc::test_pattern_generator::init() {
    canvas = std::make_unique<glp::buffer_obj>();
    if(canvas) {
        glBindBuffer(GL_ARRAY_BUFFER, canvas->id());
        std::vector<GLfloat> coords= {
            -1.0, -1.0,
            -1.0, 1.0, 
            1.0, 1.0,
            -1.0, -1.0,
            1.0, 1.0,
            1.0, -1.0,
        };

        glBufferData(GL_ARRAY_BUFFER, coords.size()*sizeof(GLfloat), coords.data(), GL_STATIC_DRAW);


        glBindBuffer(GL_ARRAY_BUFFER, 0);

        create_shaders();
    }
}


void xc::test_pattern_generator::create_shaders() {
    patterns.resize(static_cast<size_t>(xc::pattern::END));

    std::shared_ptr<glp::program> prog = std::make_shared<glp::program>();
    

    prog->load_vertex(empty_vert);
    std::string frag = "#version 120\n"
        "void main(void) {"
            "gl_FragColor = vec4(1.0,1.0,1.0,1.0);"
        "}";
    prog->load_fragment(frag);
    if(prog->compile()) {
        patterns.push_back(prog); 
    } else {
        patterns.push_back(nullptr);
    }
}


std::vector<std::string> xc::test_pattern_generator::get_pattern_names() const {
    return {
        "WHITE", "BLACK", "HOR_BARS", "VER_BARS", "RAMP", "CHECKERBOARD", "NONE"
    };
}


void xc::test_pattern_generator::select(xc::pattern p) {
    current = p;
}


void xc::test_pattern_generator::render() {
    if(canvas && patterns[0]) {
        auto coord = patterns[0]->get_attrib("coord");

        patterns[0]->attach();

        glEnableVertexAttribArray(coord);
        glBindBuffer(GL_ARRAY_BUFFER, canvas->id());
        glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(coord);
    }
}
