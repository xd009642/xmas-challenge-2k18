#ifndef XC_INPUT_LEAP_INTERFACE_H
#define XC_INPUT_LEAP_INTERFACE_H

#include "Leap.h"

namespace ip {
    
    class listener: public Leap::Listener {
    public:
        virtual void onInit(const Leap::Controller&);
        virtual void onConnect(const Leap::Controller&);
        virtual void onDisconnect(const Leap::Controller&);
        virtual void onExit(const Leap::Controller&);
        virtual void onFrame(const Leap::Controller&);
        virtual void onFocusGained(const Leap::Controller&);
        virtual void onFocusLost(const Leap::Controller&);
        virtual void onDeviceChange(const Leap::Controller&);
        virtual void onServiceConnect(const Leap::Controller&);
        virtual void onServiceDisconnect(const Leap::Controller&);
    };

    void create();
}



#endif
