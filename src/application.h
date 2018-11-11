#ifndef XC_APPLICATION_H
#define XC_APPLICATION_H

#include <cstddef>
#include "commands.h"
#include "config.h"

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
    private:
        application() noexcept;

        xc::config configuration;
        xc::command_interface cmd;
        bool running;

        int window;
    };
}

#endif
