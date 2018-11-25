#include "leap_interface.h"
#include <iostream>

Leap::Controller con;
ip::listener list;

void ip::create() {
  con.enableGesture(Leap::Gesture::TYPE_SWIPE);
  con.addListener(list);
}

void ip::listener::onInit(const Leap::Controller &con) {
}

void ip::listener::onConnect(const Leap::Controller &con) {
  std::cout << "LeapMotion connected" << std::endl;
}

void ip::listener::onDisconnect(const Leap::Controller &con) {
  std::cout << "LeapMotion disconnected" << std::endl;
}

void ip::listener::onExit(const Leap::Controller &con) {
}

void ip::listener::onFrame(const Leap::Controller &con) {
  Leap::Frame current = con.frame();
  Leap::Frame last = con.frame(1);

  auto gestures = current.gestures(last);
  for(const auto &gesture : gestures) {
    auto hands = gesture.hands();
    // Do something
  }
}

void ip::listener::onFocusGained(const Leap::Controller &con) {
}

void ip::listener::onFocusLost(const Leap::Controller &con) {
}

void ip::listener::onDeviceChange(const Leap::Controller &con) {
}

void ip::listener::onServiceConnect(const Leap::Controller &con) {
}

void ip::listener::onServiceDisconnect(const Leap::Controller &con) {
}
