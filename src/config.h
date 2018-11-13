#ifndef XC_CONFIG_H
#define XC_CONFIG_H

#include <filesystem>
#include <string>
#include <string_view>
#include "cpptoml.h"

namespace xc {

    namespace asset_folders {
        static constexpr const char* FONT_DIR = "fonts";
        static constexpr const char* SHADER_DIR = "shaders";
    }

    class config {
    public:
        bool read(const std::filesystem::path& path);
        bool write(const std::filesystem::path& path) const;

        // Getters 
        bool fullscreen() const noexcept;
        bool keyboard() const noexcept;
        const std::filesystem::path& asset_directory() const noexcept;
        std::filesystem::path font_directory() const;
        const bool has_default_font() const noexcept;
        const std::filesystem::path& default_font() const noexcept;
    protected:
        void parse_config();
    private:
        //! Directory to search for fonts
        std::filesystem::path asset_dir = "data";
        std::filesystem::path font = "";
        //! Whether the app should be shown as full screen
        bool is_fullscreen = true;
        //! Has a keyboard interface - used to disable engineering interface
        bool has_keyboard = true;
        //! debug mode 
        bool debug = false;
        std::shared_ptr<cpptoml::table> file;

    };

}


#endif
