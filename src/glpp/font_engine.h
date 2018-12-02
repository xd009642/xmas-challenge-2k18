#ifndef GLPP_FONT_ENGINE_H
#define GLPP_FONT_ENGINE_H

#include "colours.h"
#include "font_atlas.h"
#include "gl_includes.h"
#include "program.h"
#include "types.h"
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glp {

class font_engine final {
  public:
    //! Get the engine instance
    static font_engine &instance();
    //! Delete copy and move ctors
    font_engine(const font_engine &) = delete;
    font_engine(const font_engine &&) = delete;
    ~font_engine();
    //! Sets a shader program for rendering fonts with the engine
    void set_program(const program &p);
    //! Gets the atlas of the current font
    font_atlas &current_font();
    //! Load a font into the engine (currently causes prior font to unload)
    bool load(const std::filesystem::path &f);
    //! Render some 2D text at given scale
    void render_text(const std::string_view text, float x, float y, float sx,
                     float sy, colour c = green);

    std::vector<glp::mesh_2d> get_text_mesh(const std::string_view text,
                                            float x, float y, float sx,
                                            float sy);

  private:
    font_engine();

    bool initialised = false;
    bool bad_init = false;
    FT_Library ft;
    font_atlas atlas;

    std::unique_ptr<glp::buffer_obj> vbo;
    program default_shader;
};

} // namespace glp

#endif
