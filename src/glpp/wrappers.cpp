#include "wrappers.h"
#include <iostream>

GLuint glp::create_texture() {
    GLuint res;
    glGenTextures(1, &res);
    return res;
}

void glp::delete_texture(GLuint id) {
    glDeleteTextures(1, &id);
}

GLuint glp::create_array() {
    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}

void glp::delete_array(GLuint id) {
    glDeleteVertexArrays(1, &id);
}

GLuint glp::create_buffer() {
    GLuint id;
    glGenBuffers(1, &id);
    return id;
}

void glp::delete_buffer(GLuint id) {
    glDeleteBuffers(1, &id);
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

GLuint glp::create_program() {
    return glCreateProgram();
}

void glp::delete_program(GLuint id) {
    glDeleteProgram(id);
}
