#ifndef GLPP_WRAPPERS_H
#define GLPP_WRAPPERS_H

#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>

namespace glp {

    GLuint create_texture();
    void delete_texture(GLuint id);

    GLuint create_fragment_shader();
    GLuint create_vertex_shader();
    void delete_shader(GLuint id);


    using ctor_fn = decltype(create_texture);
    using dtor_fn = decltype(delete_texture);

    template<ctor_fn c, dtor_fn d>
    class gl_obj_wrapper final {
    public:
        // Create the object
        gl_obj_wrapper();
        // Copying is deleted to avoid double free
        gl_obj_wrapper(const gl_obj_wrapper<c, d>&) = delete;
        // Move ctor
        gl_obj_wrapper(gl_obj_wrapper<c, d>&&);
        // dtor
        ~gl_obj_wrapper();
        // Get the gl id
        GLuint id() const noexcept;
        // take the gl id leaving this with nothing
        GLuint take() noexcept;
    private:
        GLuint gl_id;
    };

    using texture = gl_obj_wrapper<create_texture, delete_texture>;
    using fragment_shader = gl_obj_wrapper<create_fragment_shader, delete_shader>;
    using vertex_shader = gl_obj_wrapper<create_vertex_shader, delete_shader>;

    template<ctor_fn c, dtor_fn d>
    gl_obj_wrapper<c, d>::gl_obj_wrapper():gl_id(0) {
        gl_id = c();
    }

    template<ctor_fn c, dtor_fn d>
    gl_obj_wrapper<c, d>::gl_obj_wrapper(gl_obj_wrapper<c, d>&& other) {
        gl_id = std::move(other.gl_id);
        other.gl_id = 0;
    }


    template<ctor_fn c, dtor_fn d>
    gl_obj_wrapper<c, d>::~gl_obj_wrapper() {
        if(gl_id != 0) {
            d(gl_id);
        }
    }

    template<ctor_fn c, dtor_fn d>
    GLuint gl_obj_wrapper<c,d>::id() const noexcept {
        return gl_id;
    }

    template<ctor_fn c, dtor_fn d>
    GLuint gl_obj_wrapper<c,d>::take() noexcept {
        auto res = gl_id;
        gl_id = 0;
        return res;
    }

}

#endif
