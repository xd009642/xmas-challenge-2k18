#include "commands.h"
#include "application.h"
#include <algorithm>
#include <iostream>
#include <regex>

const std::string TPG_PREFIX = "tpg";

xc::command_interface::command_interface() noexcept
    : _current(xc::command::invalid) {
  buffer.reserve(16);
}

void xc::command_interface::push_char(unsigned char c) {
  if(c == '\n' || c == '\r') {
    // Command is finished!
    if(check()) {
      execute();
    }
  } else if(c == 8 && !buffer.empty()) {
    buffer.pop_back();
  } else {
    buffer.push_back(c);
  }
}

xc::command xc::command_interface::current() const {
  return _current;
}

bool xc::command_interface::check() {

  const static auto TPG_SHOW = TPG_PREFIX + " show ";

  bool result = true;
  if(buffer == "help") {
    _current = xc::command::help;
  } else if(buffer == "clear") {
    _current = xc::command::clear;
  } else if(buffer == TPG_PREFIX + " list") {
    _current = xc::command::tpg_list;
  } else if(buffer.find(TPG_SHOW) == 0) {
    _current = xc::command::tpg_show;
    buffer.erase(0, TPG_SHOW.size());
    cmd_arg = buffer;
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
                     "Commands\n"
                     "help: shows this message\n"
                     "clear: clears messages from screen\n";
    display_string +=
        TPG_PREFIX + " list: list the available test patterns\n" + TPG_PREFIX +
        " show <PATTERN>: display a pattern using the test pattern generator\n";
  } else if(_current == xc::command::clear) {
    display_string = "";
  } else if(_current == xc::command::exit) {
    xc::application::instance().close();
  } else if(_current == xc::command::tpg_list) {
    const auto &names =
        xc::application::instance().test_pattern().get_pattern_names();
    display_string = "Available test patterns:\n";
    for(const auto &name : names) {
      display_string += name + "\n";
    }
  } else if(_current == xc::command::tpg_show) {
    xc::application::instance().test_pattern().select(cmd_arg);
    display_string.clear();
  } else {
    display_string = "Invalid command!";
  }
}

std::string xc::command_interface::display() const {
  return display_string;
}

std::string xc::command_interface::cmd_string() const {
  return buffer;
}
