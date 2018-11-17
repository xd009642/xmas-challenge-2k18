#include "program.h"


namespace fs = std::filesystem;

bool glp::program::load_fragment(const fs::path& file) {
    return false;
}


bool glp::program::load_vertex(const fs::path& file) {
    return false;
}


bool glp::program::compile() {
    return false;
}


void glp::program::attach() {
    if(id != 0) {
        glUseProgram(id);
    }
}
