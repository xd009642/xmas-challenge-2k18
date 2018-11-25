#include "heading_tape.h"
#include "mesh_util.h"

#include <iostream>

void sym::heading_tape::init() {
  mesh = std::make_unique<glp::buffer_obj>();
  if(mesh) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh->id());
    auto points = sym::make_line_mesh({glp::point<GLfloat>(0.8f, 0.8f),
                                       glp::point<GLfloat>(0.0f, 0.6f),
                                       glp::point<GLfloat>(-0.8f, 0.8f)},
                                      0.0f);

    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glp::point<GLfloat>),
                 points.data(), GL_STATIC_DRAW);

    size = points.size() * 2;

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string vert = "#version 120\n"
                       "attribute vec2 coord;"
                       "void main(void) {"
                       "gl_Position = vec4(coord.xy, 0.0, 1.0);"
                       "}";

    std::string frag =
        "#version 120\n"
        "void main(void) { gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); }";

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

    shader->attach();
    glEnableVertexAttribArray(coord);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->id());
    glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, size * 2);
    shader->detach();
  }
}
