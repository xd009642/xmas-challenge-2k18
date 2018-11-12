#ifndef GLPP_FONT_ATLAS_H
#define GLPP_FONT_ATLAS_H

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glp {
    //! Texture atlas class
    class font_atlas {
    public:
        font_atlas() = default;
        //! Construct an atlas given a face and a font size (pixels)
        font_atlas(FT_Face face, size_t font_size);
        ~font_atlas();

        bool valid() const;

    private:
        bool is_valid = false;
        unsigned int block_width = 0;
        unsigned int block_height = 0;
        size_t n_blocks = 0;

        static constexpr char MIN_CHAR = ' ';
        static constexpr char MAX_CHAR = '~';
        std::string loaded_chars;
    };
    
}

#endif
