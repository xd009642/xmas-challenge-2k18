#ifndef XC_CONFIG_H
#define XC_CONFIG_H

#include <string_view>

namespace xc {

    class config {
    public:
        bool read(const std::string_view path);
        bool write(const std::string_view path) const;

    private:
        bool changed = true;

        bool is_fullscreen = true;
    };

}


#endif
