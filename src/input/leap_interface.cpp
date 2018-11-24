#include "leap_interface.h"
#include <iostream>

Leap::Controller con;
ip::listener list;

void ip::create() {
    con.addListener(list);
}


void ip::listener::onInit(const Leap::Controller& con) {
}

void ip::listener::onConnect(const Leap::Controller& con) {
}

void ip::listener::onDisconnect(const Leap::Controller& con) {
}

void ip::listener::onExit(const Leap::Controller& con) {
}

void ip::listener::onFrame(const Leap::Controller& con) {
}

void ip::listener::onFocusGained(const Leap::Controller& con) {
}

void ip::listener::onFocusLost(const Leap::Controller& con) {
}

void ip::listener::onDeviceChange(const Leap::Controller& con) {
}

void ip::listener::onServiceConnect(const Leap::Controller& con) {
}

void ip::listener::onServiceDisconnect(const Leap::Controller& con) {
}
