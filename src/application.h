#ifndef XC_APPLICATION_H
#define XC_APPLICATION_H

#include <cstddef>
#include "commands.h"
#include "config.h"
#include "glpp/font_engine.h"

namespace xc {
    //! Singleton representing the application
    class application final {
    public:
        static application& instance() noexcept;
        application(const application&) = delete;
        application(const application&&) = delete;
        //! Initialises the application
        void init();
        //! Starts the applications update loop
        void start();
        
        void close();
        
        command_interface& command();

        glp::font_engine& fonts();
    private:
        application() noexcept;

        glp::font_engine font_eng;
        xc::config config;
        xc::command_interface cmd;
        bool running;

        int window;
    };
}

#endif
