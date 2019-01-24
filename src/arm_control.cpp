#include "arm_control.h"
#include <iostream>
#include <boost/system/error_code.hpp>

namespace ba = boost::asio;

bool xc::arm_controller::init() {
    std::string port_name = "/dev/ttyACM0";
    return init(port_name);    
}
        
bool xc::arm_controller::init(const std::string& name) {
    command_bytes[commands::win] = 0;
    command_bytes[commands::lose] = 1;

    port.reset(new ba::serial_port(io));
    if(port) {
        boost::system::error_code e;
        port->open(name, e);
        if(e) {
            std::cout<<"[ERROR] Failed to open port "<<name<<" "<<e.message().c_str()<<std::endl;
        } else {
            port->set_option(ba::serial_port_base::baud_rate(115200));
            port->set_option(ba::serial_port_base::character_size(8));
            port->set_option(ba::serial_port_base::stop_bits(ba::serial_port_base::stop_bits::one));
            port->set_option(ba::serial_port_base::parity(ba::serial_port_base::parity::none));
            port->set_option(ba::serial_port_base::flow_control(ba::serial_port_base::flow_control::none));

            io.run();
            std::cout<<"[INFO] Arm client set up"<<std::endl;
            ready = true;
            return true;
        }
    }
    return false;
}

void xc::arm_controller::grab_win() {
    if(ready && port) {
        boost::system::error_code e;
        auto res = port->write_some(ba::buffer(command_bytes + commands::win, 1), e);
        if(res != 1) {
            std::cout<<"[ERROR] serial port write error "<<e.message().c_str()<<std::endl;
        }
    }
}

void xc::arm_controller::grab_lose() {
    if(ready && port) {
        boost::system::error_code e;
        auto res = port->write_some(ba::buffer(command_bytes + commands::lose, 1), e);
        if(res != 1) {
            std::cout<<"[ERROR] serial port write error "<<e.message().c_str()<<std::endl;
        }
    }
}

