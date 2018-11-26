#ifndef GLPP_PROGRAM_H
#define GLPP_PROGRAM_H

#include "gl_includes.h"
#include "wrappers.h"

#include <filesystem>
#include <string>
#include <vector>

namespace glp {
//! Wrapper around a shader program
class program {
public:
  //! Load a fragment shader from a file
  bool load_fragment(const std::filesystem::path &file);
  //! Load a fragment shader from a string
  bool load_fragment(const std::string &frag);
  //! Load a vertex shader
  bool load_vertex(const std::filesystem::path &file);
  //! Load a vertex shader from a string
  bool load_vertex(const std::string &vert);
  //! Compiles the current program
  bool compile();
  //! Uses the current shader program for rendering
  void attach();
  //! Detaches the current program
  void detach();
  //! If compiled and linked return true
  bool valid() const noexcept;
  //! The shader programs id
  GLuint handle() const noexcept;
  //! Get location from uniform name
  GLint get_uniform(const std::string &name);
  //! Get location from attribute name
  GLint get_attrib(const std::string &name);
  //! Prints a list of all uniforms
  void print_uniforms();

protected:
  //! Given a shader id and source compile the shader
  bool compile_shader(GLuint sid, const std::string &source);
  //! Gets the shader info log
  std::string get_info_log(GLuint shader);

private:
  std::shared_ptr<program_obj> prog;
  std::shared_ptr<fragment_shader> fragment;
  std::shared_ptr<vertex_shader> vertex;
};
} // namespace glp

#endif
