#include "heading_tape.h"
#include "mesh_util.h"

#include <iostream>

void sym::heading_tape::init() {
    mesh = std::make_unique<glp::buffer_obj>();
    if(mesh) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->id());
        auto points = sym::make_scale_line(glp::point<GLfloat>(-1.0f, 0.8f),
                                           glp::point<GLfloat>(1.1f, 0.8f),
                                           0.1f, 0.004f, 0.1f);
        auto minor_points = sym::make_scale_line(
            glp::point<GLfloat>(-1.0f, 0.8f), glp::point<GLfloat>(1.0f, 0.8f),
            0.02f, 0.004f, 0.05f);

        points.insert(points.end(), minor_points.begin(), minor_points.end());

        glBufferData(GL_ARRAY_BUFFER,
                     points.size() * sizeof(glp::point<GLfloat>), points.data(),
                     GL_STATIC_DRAW);

        size = points.size() * 2;

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::string vert = "#version 120\n"
                           "attribute vec2 coord;\n"
                           "uniform float width;\n"
                           "varying float is_on;\n"
                           "void main(void) {\n"
                           "is_on = float(width > abs(coord.x));"
                           "gl_Position = vec4(coord.xy, 0.0, 1.0);\n"
                           "}";

        std::string frag = "#version 120\n"
                           "varying float is_on;\n"
                           "void main(void) {\n"
                           "gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0) * is_on + vec4(1.0,1.0,1.0,1.0)*(1.0-is_on);\n"
                           "}";

        shader = std::make_shared<glp::program>();
        if(shader) {
            shader->load_vertex(vert);
            shader->load_fragment(frag);
            shader->compile();
        }
    }
}

void sym::heading_tape::render() {
    if(mesh && shader) {
        auto coord = shader->get_attrib("coord");

        GLint width = shader->get_uniform("width");
        shader->attach();

        glEnableVertexAttribArray(coord);

        glUniform1f(width, 0.5f);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->id());
        glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, size * 2);
        shader->detach();
    }
}
