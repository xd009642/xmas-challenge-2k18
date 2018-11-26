#include "program.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

bool glp::program::load_fragment(const fs::path &file) {
    bool result = false;
    if(file.has_filename() && fs::exists(file)) {
        std::ifstream f(file);
        std::stringstream buffer;
        buffer << f.rdbuf();
        result = load_fragment(buffer.str());
    } else {
        std::cout << "Failed to find shader source " << file << std::endl;
    }
    return result;
}

bool glp::program::load_fragment(const std::string &source) {
    bool result = false;
    fragment = std::make_shared<glp::fragment_shader>();
    if(fragment) {
        GLuint id = fragment->id();
        if(!compile_shader(id, source)) {
            std::cout << "Fragment Shader ERROR: " << get_info_log(id)
                      << std::endl;
            fragment.reset();
            result = false;
        } else {
            result = true;
        }
    }
    return result;
}

bool glp::program::load_vertex(const fs::path &file) {
    bool result = false;
    if(file.has_filename() && fs::exists(file)) {
        std::ifstream f(file);
        std::stringstream buffer;
        buffer << f.rdbuf();
        result = load_vertex(buffer.str());
    } else {
        std::cout << "Failed to find shader source " << file << std::endl;
    }
    return result;
}

bool glp::program::load_vertex(const std::string &source) {
    bool result = false;
    vertex = std::make_shared<glp::vertex_shader>();
    if(vertex) {
        GLuint id = vertex->id();
        if(!compile_shader(id, source)) {
            std::cout << "Vertex Shader ERROR: " << get_info_log(id)
                      << std::endl;
            vertex.reset();
            result = false;
        } else {
            result = true;
        }
    }

    return result;
}

bool glp::program::compile_shader(GLuint sid, const std::string &source) {
    const char *temp = source.c_str();
    glShaderSource(sid, 1, &temp, NULL);
    glCompileShader(sid);

    int is_good;
    glGetShaderiv(sid, GL_COMPILE_STATUS, &is_good);
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
    prog.reset();
    if(vertex && fragment) {
        prog = std::make_shared<glp::program_obj>();
        if(prog) {
            GLuint id = prog->id();
            glAttachShader(id, vertex->id());
            glAttachShader(id, fragment->id());

            glLinkProgram(id);

            int success;
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if(!success) {
                std::string proglog;
                GLint size;
                glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);
                proglog.resize(size);
                glGetProgramInfoLog(id, size, &size, proglog.data());
                prog.reset();
                std::cout << "Failed to compile program: " << proglog
                          << std::endl;
            }
            // now they're linked don't need them
            glDetachShader(id, fragment->id());
            glDetachShader(id, vertex->id());
            fragment.reset();
            vertex.reset();
        }
    }
    return static_cast<bool>(prog);
}

void glp::program::attach() {
    if(prog) {
        glUseProgram(prog->id());
    } else {
        std::cout << "Failed to attach shader" << std::endl;
    }
}

void glp::program::detach() {
    glUseProgram(0);
}

bool glp::program::valid() const noexcept {
    return static_cast<bool>(prog);
}

GLuint glp::program::handle() const noexcept {
    if(prog) {
        return prog->id();
    } else {
        return 0;
    }
}

GLint glp::program::get_uniform(const std::string &name) {
    if(prog) {
        return glGetUniformLocation(prog->id(), name.data());
    } else {
        std::cout << "Trying to get uniform when no program's been compiled"
                  << std::endl;
    }
    return -1;
}

GLint glp::program::get_attrib(const std::string &name) {
    if(prog) {
        return glGetAttribLocation(prog->id(), name.data());
    } else {
        std::cout << "Trying to get attrib when no program's been compiled"
                  << std::endl;
    }
    return -1;
}

void glp::program::print_uniforms() {
    if(prog) {
        // If I use a name larger than this I should probably be shot
        const int MAX_NAME_LEN = 25;
        GLchar name[MAX_NAME_LEN];
        GLint count;

        glGetProgramiv(prog->id(), GL_ACTIVE_UNIFORMS, &count);
        std::cout << "Program contains " << count << " uniform(s)" << std::endl;
        for(GLint i = 0; i < count; i++) {
            glGetActiveUniform(prog->id(), static_cast<GLuint>(i), MAX_NAME_LEN,
                               nullptr, nullptr, nullptr, name);

            std::cout << name << std::endl;
        }
    }
}
