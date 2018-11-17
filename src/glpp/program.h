#ifndef GLPP_PROGRAM_H
#define GLPP_PROGRAM_H


#include "gl_includes.h"
#include "wrappers.h"

#include <filesystem>

namespace glp {

    class program {
    public:

        //! Load a fragment shader
        bool load_fragment(const std::filesystem::path& file);
        //! Load a vertex shader
        bool load_vertex(const std::filesystem::path& file);
        //! Compiles the current program
        bool compile();
        //! Uses the current shader program for rendering
        void attach();
    private:
        GLuint id = 0;
        
        std::shared_ptr<fragment_shader> fragment;
        std::shared_ptr<vertex_shader> vertex;
    };
}

#endif
