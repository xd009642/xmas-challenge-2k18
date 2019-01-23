#ifndef XC_ARM_CONTROL_H
#define XC_ARM_CONTROL_H

#include <memory>
#include <string>
#include <boost/asio.hpp>

namespace xc {

    class arm_controller {
    public:
        //! Attempts to find a serial port to communicate with
        bool init();
        //! init given the port name 
        bool init(const std::string& name);
        //! Grab winner tile (arm has two tokens)
        void grab_win();
        //! Grab the loser tile
        void grab_lose();
    private:
        enum commands {
            win, lose, end
        };
        bool ready;
    
        boost::asio::io_service io;
        std::unique_ptr<boost::asio::serial_port> port;

        uint8_t command_bytes[commands::end];
    };

}


#endif

