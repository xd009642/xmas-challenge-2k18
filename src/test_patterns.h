#ifndef XC_TEST_PATTERNS_H
#define XC_TEST_PATTERNS_H

#include "glpp/program.h"
#include "glpp/wrappers.h"

#include <memory>
#include <string>
#include <vector>

namespace xc {

enum class pattern {
    WHITE,
    BLACK,
    HOR_BARS,
    VER_BARS,
    RAMP,
    CHECKERBOARD,
    NONE,
    END
};

class test_pattern_generator {
  public:
    test_pattern_generator() = default;

    void init();
    void select(const std::string &pattern_name);
    void select(pattern p);
    void render();
    std::vector<std::string> get_pattern_names() const;

  protected:
    void create_shaders();
    void create_flat();
    void create_bars();
    void create_ramp();
    void create_checkerboard();

    void create_shader(pattern p, const std::string &fragment_source);

  private:
    pattern current = pattern::NONE;

    std::vector<std::shared_ptr<glp::program>> patterns;

    std::unique_ptr<glp::buffer_obj> canvas;
};
} // namespace xc

#endif
