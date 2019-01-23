#ifndef XC_APPLICATION_H
#define XC_APPLICATION_H

#include "arm_control.h"
#include "commands.h"
#include "config.h"
#include "glpp/font_engine.h"
#include "test_patterns.h"
#include <cstddef>
#include <string>
#include <vector>

namespace xc {
//! Singleton representing the application
class application final {
  public:
    static application &instance();
    application(const application &) = delete;
    application(const application &&) = delete;
    //! Initialises the application
    void init();
    //! Starts the applications update loop
    void start();

    void close();

    std::vector<std::string> get_available_fonts() const;

    command_interface &command();

    test_pattern_generator &test_pattern();

    arm_controller& arm_control();
  private:
    application();

    void init_graphics();
    void init_fonts();

    xc::arm_controller arm;
    glp::font_engine &fonts;
    xc::config config;
    xc::command_interface cmd;
    xc::test_pattern_generator tpg;
    bool running;

    int window;
};
} // namespace xc

#endif
