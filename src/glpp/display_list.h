#ifndef GLPP_DISPLAY_LIST
#define GLPP_DISPLAY_LIST

#include "generic_lock.h"
#include "gl_includes.h"

namespace glp {

//! Safe wrapper around an OpenGL display list.
//! Because these are a list of list of commands I won't both using a wrapper
//! class
class display_list final {
public:
  //! Mode to use when creating a new list
  enum class mode {
    compile = GL_COMPILE,
    compile_and_execute = GL_COMPILE_AND_EXECUTE
  };
  //! Constructs 0..N display lists. 0 will do nothing so 1 is the default
  display_list(size_t range = 1);
  ~display_list();

  generic_lock new_list(size_t index, mode m = mode::compile);

  void call_list(size_t index = 0);

private:
  const size_t range;
  GLuint base_index;
};

} // namespace glp

#endif
