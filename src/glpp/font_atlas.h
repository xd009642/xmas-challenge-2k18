#ifndef GLPP_FONT_ATLAS_H
#define GLPP_FONT_ATLAS_H

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glp {
    //! Texture atlas class
    class font_atlas {
    public:
        font_atlas(FT_Face& face);
        ~font_atlas();

        bool valid() const;

    private:
        bool is_valid = false;
        size_t block_width;
        size_t block_height;
        size_t n_blocks;
    };
    
}

#endif
