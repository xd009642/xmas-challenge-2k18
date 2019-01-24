#ifndef COMMANDS_H
#define COMMANDS_H

#include <array>
#include <string>
#include <string_view>

namespace xc {
/**
 * Commands for engineering mode each command is a line :<STRING>\n
 */
enum class command {
    // Show help message
    help,
    // Clears the engineering interface
    clear,
    exit,
    tpg_list,
    tpg_show,
    grab_win,
    grab_lose,
    invalid
};

class command_interface {
  public:
    command_interface() noexcept;

    command current() const;
    void push_char(unsigned char c);
    // If command is invalid error message will be in display string?
    std::string display() const;

    std::string cmd_string() const;

  private:
    bool check();
    void execute();

    std::string buffer;
    std::string display_string;

    std::string cmd_arg;
    command _current;
};
} // namespace xc

#endif
