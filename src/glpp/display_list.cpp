#include "display_list.h"
#include <exception>


glp::display_list::display_list(size_t range):range(range) {
    base_index = glGenLists(range);
}

glp::display_list::~display_list() {
    glDeleteLists(base_index, range);
}

glp::generic_lock glp::display_list::new_list(size_t index,
        glp::display_list::mode mode) {

    if(index >= base_index+range || range == 0) {
        throw std::out_of_range("Display list index out of range");
    }

    glNewList(base_index + index, static_cast<GLenum>(mode));
    return glp::generic_lock(glEndList);
}

void glp::display_list::call_list(size_t index) {
    if(index >= base_index+range || range == 0) {
        throw std::out_of_range("Display list index out of range");
    }
    glCallList(base_index+index);
}
