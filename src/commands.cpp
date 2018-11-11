#include "commands.h"
#include <algorithm>
#include "application.h"


xc::command_interface::command_interface() noexcept:
_current(xc::command::invalid) {
    buffer.reserve(16);
}


void xc::command_interface::push_char(unsigned char c) {
    if(c == '\n' || c == '\r') {
        // Command is finished!
        if(check()) {
            execute();
        }
    } else if (c == 8) {
        buffer.pop_back();  
    } else {
        buffer.push_back(c);
    }
}

xc::command xc::command_interface::current() const {
    return _current;
}

bool xc::command_interface::check() {
    bool result = true;
    if(buffer == "help") {
        _current = xc::command::help;
    } else if(buffer == "clear") {
        _current = xc::command::clear;
    } else {
        result = false;
        _current = xc::command::invalid;
    }
    buffer.clear();
    return result;
}

void xc::command_interface::execute() {
    if(_current == xc::command::help) {
        display_string = "Welcome to the xmas engineering interface\n\n"
            "Commands:\n"
            "help -- shows this message\n"
            "clear -- clears messages from screen\n"
            "exit -- quits the program";
    } else if(_current == xc::command::clear) {
        display_string= "";
    } else if(_current == xc::command::exit) {
        xc::application::instance().close();  
    } else {
        display_string = "Invalid command!";
    }
    
}

const std::string_view xc::command_interface::display() const {
    return display_string;
}

const std::string_view xc::command_interface::cmd_string() const {
    return buffer;
}
