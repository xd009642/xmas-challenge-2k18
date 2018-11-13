#ifndef GLPP_FONT_ATLAS_H
#define GLPP_FONT_ATLAS_H

#include <string>

#include "GL/gl.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "generic_lock.h"
#include "types.h"

namespace glp {
    //! Texture atlas class
    class font_atlas {
    public:
        font_atlas() = default;
        //! Construct an atlas given a face and a font size (pixels)
        font_atlas(FT_Face face, size_t font_size);
        ~font_atlas();
        //! Does the atlas contain a valid texture?
        bool valid() const;
        //! Binds the texture, unbinding when the lock is out of scope
        generic_lock bind();
        //! Does it contain the following character?
        bool contains(const char c) const;

        rect<double> get_coords(const char c) const;
    private:
        bool is_valid = false;
        unsigned int block_width = 0;
        unsigned int block_height = 0;

        static constexpr char MIN_CHAR = ' ';
        static constexpr char MAX_CHAR = '~';
        std::string loaded_chars;

        GLuint tex_id;
    };
    
}

#endif
