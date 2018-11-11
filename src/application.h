#ifndef XC_APPLICATION_H
#define XC_APPLICATION_H

#include <cstddef>

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
    private:
        application() noexcept;

        bool running;
    };
}

#endif
