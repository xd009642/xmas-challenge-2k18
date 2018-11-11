#include "font_engine.h"
#include <iostream>


bool glp::font_engine::initialised = false;
bool glp::font_engine::bad_init = false;
FT_Library glp::font_engine::ft = {};

glp::font_engine::font_engine() {
    if(!initialised && !bad_init) {
        if(FT_Init_FreeType(&ft)) {
            std::cout<<"Failed to initialise freetype"<<std::endl;
            bad_init = true;
        } else {
            std::cout<<"Initialised font_engine"<<std::endl;
            initialised = true;
        }
    }
}
