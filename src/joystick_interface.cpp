#include "joystick_interface.h"
#include "linux/joystick.h"
#include "fcntl.h"
#include "unistd.h"
#include <iostream>
#include <filesystem>
#include "linux/joystick.h"


std::string joystick_path(size_t i) {
    return "/dev/input/js"+std::to_string(i);
}

xc::joystick::joystick(size_t index) noexcept {
    this->index = index;
    is_running = false;
}

xc::joystick::~joystick() {
    is_running = false;
    update.join();
    close(fd);
}


bool xc::joystick::is_available() const {
    return std::filesystem::exists(joystick_path(index));
}


bool xc::joystick::connect() {
    fd = open(joystick_path(index).c_str(), O_RDONLY);
    if(fd != -1) {
        is_running = true;
        update  = std::thread([&]() {
            js_event e;
            int size;
            while(is_running) {
                size = read(fd, &e, sizeof(e));
                if(size == sizeof(e)) {
                    std::lock_guard<std::mutex> guard(lock);
                    // ignore synthetic events 
                    e.type &= ~JS_EVENT_INIT;
                    if(e.type == JS_EVENT_BUTTON) {
                        state.buttons[static_cast<size_t>(e.number)] = e.value;
                    } else if(e.type == JS_EVENT_AXIS) {
                        switch(e.number) {
                        case 0: 
                            state.left_stick.x = e.value;
                        break;
                        case 1: 
                            state.left_stick.y = e.value;
                        break;
                        case 2: 
                            state.right_stick.x = e.value;
                        break;
                        case 3: 
                            state.right_stick.y = e.value;
                        break;
                        }
                    }
                }
            }
        });
    }
    return fd != -1;
}

bool xc::joystick::is_good() const {
    return fd != -1;
}

xc::stick_state xc::joystick::get_state() const {
    std::lock_guard guard(lock);
    return state;
}
    

void xc::print_state(const stick_state& state) {
    std::cout<<"TRI\t"<<state.buttons[TRIANGLE]<<std::endl;    
    std::cout<<"CIR\t"<<state.buttons[CIRCLE]<<std::endl;    
    std::cout<<"CROSS\t"<<state.buttons[CROSS]<<std::endl;    
    std::cout<<"SQR\t"<<state.buttons[SQUARE]<<std::endl;    
    std::cout<<"L1\t"<<state.buttons[L1]<<std::endl;    
    std::cout<<"L2\t"<<state.buttons[L2]<<std::endl;    
    std::cout<<"R1\t"<<state.buttons[R1]<<std::endl;    
    std::cout<<"R2\t"<<state.buttons[R2]<<std::endl;
    std::cout<<"Left stick "<<state.left_stick.x
        <<":"<<state.left_stick.y<<std::endl;
    std::cout<<"Right stick "<<state.right_stick.x
        <<":"<<state.right_stick.y<<std::endl;
}
