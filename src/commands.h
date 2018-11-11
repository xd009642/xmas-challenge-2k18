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
        invalid
    };

    class command_interface {
    public:
        command_interface() noexcept;

        command current() const;
        void push_char(unsigned char c);
        // If command is invalid error message will be in display string?
        const std::string_view display() const;

        const std::string_view cmd_string() const;
    private:
        bool check();
        void execute();

        std::string buffer;
        std::string display_string;

        command _current;
    };
}


#endif
