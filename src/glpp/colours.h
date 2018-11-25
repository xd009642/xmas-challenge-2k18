#ifndef GLPP_COLOURS_H
#define GLPP_COLOURS_H

#include "gl_includes.h"
#include <array>

namespace glp {

using colour = std::array<GLfloat, 4>;

constexpr colour create_colour(float r, float g, float b, float a = 1.0f) {
  return {r, g, b, a};
}

constexpr colour green = create_colour(0.0f, 1.0f, 0.0f);
constexpr colour blue = create_colour(0.0f, 0.0f, 1.0f);
constexpr colour red = create_colour(1.0f, 0.0f, 0.0f);
constexpr colour white = create_colour(1.0f, 1.0f, 1.0f);
constexpr colour black = create_colour(0.0f, 0.0f, 0.0f);

} // namespace glp

#endif
