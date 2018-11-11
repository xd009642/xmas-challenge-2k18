#ifndef XC_CONFIG_H
#define XC_CONFIG_H

#include <filesystem>
#include <string>
#include <string_view>
#include "cpptoml.h"

namespace xc {

    class config {
    public:
        bool read(const std::filesystem::path& path);
        bool write(const std::filesystem::path& path) const;

    private:
        //! Directory to search for fonts
        std::filesystem::path font_dir = "data";
        //! Whether the app should be shown as full screen
        bool is_fullscreen = true;

        std::shared_ptr<cpptoml::table> file;
    };

}


#endif
