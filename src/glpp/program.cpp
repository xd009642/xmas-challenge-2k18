#include "program.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


namespace fs = std::filesystem;

bool glp::program::load_fragment(const fs::path& file) {
    bool result = false;
    if(file.has_filename() && fs::exists(file)) {
        std::ifstream f(file);
        std::stringstream buffer;
        buffer << f.rdbuf();
       
        fragment = std::make_shared<glp::fragment_shader>();
        if(fragment) {
            GLuint id = fragment->id();
            std::string source = buffer.str();
            if(!load_shader(id, source)) {
                std::cout<<"Fragment Shader ERROR: "<<get_info_log(id)<<std::endl;
                fragment.reset();
                result = false;
            } else {
                result = true;
            }
        }
    }
    return result;
}


bool glp::program::load_vertex(const fs::path& file) {
    bool result = false;
    if(file.has_filename() && fs::exists(file)) {
        std::ifstream f(file);
        std::stringstream buffer;
        buffer << f.rdbuf();
       
        vertex = std::make_shared<glp::vertex_shader>();
        if(vertex) {
            GLuint id = vertex->id();
            std::string source = buffer.str();
            if(!load_shader(id, source)) {
                std::cout<<"Vertex Shader ERROR: "<<get_info_log(id)<<std::endl;
                vertex.reset();
                result = false;
            } else {
                result = true;
            }
        }
    }
    return result;
}

bool glp::program::load_shader(GLuint sid, const std::string& source) {
    const char* temp = source.c_str();
    glShaderSource(id, 1, &temp, NULL);
    glCompileShader(id);

    int is_good;
    glGetShaderiv(id, GL_COMPILE_STATUS, &is_good);
    return static_cast<bool>(is_good); 
}


std::string glp::program::get_info_log(GLuint shader) {
    std::string result;
    GLint size;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
    result.resize(size);
    glGetShaderInfoLog(shader, size, nullptr, result.data());
    return result;
}


bool glp::program::compile() {
    if(vertex && fragment) {
        id = glCreateProgram();
        glAttachShader(id, vertex->id());
        glAttachShader(id, fragment->id());
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            id = 0;
            std::cout<<"Failed to compile program"<<std::endl;
        } 
        fragment.reset();
        vertex.reset();
    }
    return false;
}


void glp::program::attach() {
    if(id != 0) {
        glUseProgram(id);
    }
}


void glp::program::detach() {
    glUseProgram(0);
}
        
bool glp::program::valid() const noexcept {
    return id != 0;
}


GLuint glp::program::handle() const noexcept {
    return id;
}
