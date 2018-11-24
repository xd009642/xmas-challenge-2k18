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

        //! Whether the application should be fullscreen
        bool fullscreen() const noexcept;
        //! Keyboard is enabled 
        bool keyboard() const noexcept;
        //! Path to the programs asset directory
        const std::filesystem::path& asset_directory() const noexcept;
        //! Path to the programs font directory - a subdirectory of the asset
        //! directory
        std::filesystem::path font_directory() const;
        //! Whether the system has a default font.
        const bool has_default_font() const noexcept;
        //! Path to the default font or empty if there isn't one
        const std::filesystem::path& default_font() const noexcept;
    
        void set_default_font(const std::filesystem::path& font);
    protected:
        //! Parse the configuration file
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
