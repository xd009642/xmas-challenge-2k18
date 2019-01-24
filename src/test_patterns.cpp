#include "test_patterns.h"
#include <algorithm>
#include <iostream>

static std::string empty_vert = "#version 120\n"
                                "attribute vec2 coord;"
                                "varying vec2 pos;"
                                "void main(void) {"
                                "   gl_Position = vec4(coord.xy, 0.0, 1.0);"
                                "   pos = coord.xy;"
                                "}";

void xc::test_pattern_generator::init() {
    canvas = std::make_unique<glp::buffer_obj>();
    if(canvas) {
        glBindBuffer(GL_ARRAY_BUFFER, canvas->id());
        std::vector<GLfloat> coords = {
            -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0,
        };

        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(GLfloat),
                     coords.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        create_shaders();
    }
}

void xc::test_pattern_generator::create_shaders() {
    patterns.resize(static_cast<size_t>(xc::pattern::END));

    create_flat();
    create_bars();
    create_ramp();
    create_checkerboard();
}

void xc::test_pattern_generator::create_flat() {

    std::string frag = "#version 120\n"
                       "void main(void) {"
                       "gl_FragColor = vec4(1.0,1.0,1.0,1.0);"
                       "}";

    create_shader(xc::pattern::WHITE, frag);

    frag = "#version 120\n"
           "void main(void) {"
           "gl_FragColor = vec4(0.0,0.0,0.0,1.0);"
           "}";

    create_shader(xc::pattern::BLACK, frag);
}

void xc::test_pattern_generator::create_bars() {
    std::string frag = "#version 120\n"
                       "varying vec2 pos;"
                       "void main(void) {"
                       "    float alpha = sin(pos.x * 50);"
                       "    gl_FragColor = vec4(1.0,1.0,1.0, step(0.0, alpha));"
                       "}";

    create_shader(xc::pattern::HOR_BARS, frag);

    frag = "#version 120\n"
           "varying vec2 pos;"
           "void main(void) {"
           "    float alpha = sin(pos.y * 50);"
           "    gl_FragColor = vec4(1.0,1.0,1.0, step(0.0, alpha));"
           "}";

    create_shader(xc::pattern::VER_BARS, frag);
}

void xc::test_pattern_generator::create_ramp() {
    std::string frag = "#version 120\n"
                       "varying vec2 pos;"
                       "void main(void) {"
                       "   float y_ramp = 0.5*pos.y + 0.5;"
                       "   gl_FragColor = vec4(1.0,1.0,1.0, y_ramp);"
                       "}";

    create_shader(xc::pattern::RAMP, frag);
}

void xc::test_pattern_generator::create_checkerboard() {
    std::string frag =
        "#version 120\n"
        "varying vec2 pos;"
        "void main(void) {"
        "   const int sz = 5;"
        "   float res = mod(floor(sz*pos.x) + floor(sz*pos.y), 2);"
        "   gl_FragColor = vec4(1.0,1.0,1.0, max(sign(res), 0.0));"
        "}";

    create_shader(xc::pattern::CHECKERBOARD, frag);
}

void xc::test_pattern_generator::create_shader(
    xc::pattern p, const std::string &fragment_source) {

    std::shared_ptr<glp::program> prog = std::make_shared<glp::program>();

    prog->load_vertex(empty_vert);
    prog->load_fragment(fragment_source);
    if(prog->compile()) {
        const size_t index = static_cast<size_t>(p);
        if(index < patterns.size()) {
            patterns[index] = prog;
        }
    }
}

std::vector<std::string> xc::test_pattern_generator::get_pattern_names() const {
    return {"WHITE", "BLACK",        "HOR_BARS", "VER_BARS",
            "RAMP",  "CHECKERBOARD", "NONE"};
}

void xc::test_pattern_generator::select(const std::string &pattern_name) {
    auto names = get_pattern_names();
    auto it = std::find(names.begin(), names.end(), pattern_name);
    if(it != names.end()) {
        current = static_cast<xc::pattern>(std::distance(names.begin(), it));
    } else {
        current = xc::pattern::NONE;
    }
}

void xc::test_pattern_generator::select(xc::pattern p) {
    current = p;
}

void xc::test_pattern_generator::render() {
    size_t current_index = static_cast<size_t>(current);
    if(canvas && patterns[current_index]) {
        auto coord = patterns[current_index]->get_attrib("coord");

        patterns[current_index]->attach();

        glEnableVertexAttribArray(coord);
        glBindBuffer(GL_ARRAY_BUFFER, canvas->id());
        glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(coord);
    }
}
