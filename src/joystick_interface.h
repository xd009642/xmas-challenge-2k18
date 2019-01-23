#ifndef XC_JOYSTICK_INTERFACE_H
#define XC_JOYSTICK_INTERFACE_H

#include <mutex>
#include <atomic>
#include <thread>

namespace xc {
    
    struct analog_input {
        int16_t x = 0;
        int16_t y = 0;
    };

    enum key_codes {
        TRIANGLE = 0,
        CIRCLE = 1,
        CROSS = 2,
        SQUARE = 3,
        L2 = 4,
        R2 = 5,
        L1 = 6,
        R1 = 7,
        SELECT = 8,
        START = 9,
        KEY_END
    };

    struct stick_state {
        bool buttons[static_cast<size_t>(key_codes::KEY_END)];
        analog_input right_stick;
        analog_input left_stick;
    };
    
    void print_state(const stick_state& state);

    class joystick {
    public:
        joystick(size_t index=0) noexcept;
        joystick(const joystick&) = delete;
        
        ~joystick();

        joystick& operator=(const joystick&) = delete;
        joystick& operator=(const joystick&) volatile = delete;
    
        bool is_available() const;
        bool connect();
        bool is_good() const;
        // Go for a copy just to avoid thread faff
        stick_state get_state() const; 
 
    private:
        stick_state state;
        int fd = -1;
        size_t index;

        mutable std::mutex lock;
        std::atomic<bool> is_running;
        std::thread update;
    };
}


#endif 
