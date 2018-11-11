#include "font_atlas.h"



glp::font_atlas::font_atlas(FT_Face& face) {

}

glp::font_atlas::~font_atlas() {

}


bool font_atlas::valid() const {
    return is_valid;
}
