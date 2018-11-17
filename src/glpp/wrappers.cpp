#include "wrappers.h"


GLuint glp::create_texture() {
    GLuint res;
    glGenTextures(1, &res);
    return res;
}

void glp::delete_texture(GLuint id) {
    glDeleteTextures(1, &id);
}

GLuint glp::create_fragment_shader() {
    return glCreateShader(GL_FRAGMENT_SHADER);
}

GLuint glp::create_vertex_shader() {
    return glCreateShader(GL_VERTEX_SHADER);
}

void glp::delete_shader(GLuint id) {
    glDeleteShader(id);
}
