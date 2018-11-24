#include "config.h"
#include <iostream>

namespace fs = std::filesystem;



bool xc::config::read(const fs::path& path) {
    if(path.has_filename() && fs::exists(path)) {
        try {
            std::cout<<"Parsing config"<<std::endl;
            file = cpptoml::parse_file(path); 
            parse_config();
            return true;
        } catch(const cpptoml::parse_exception e) {
            std::cerr<<"Failed to parse config file: "<<e.what()<<std::endl;
        }
    }
    return false;
}

bool xc::config::write(const fs::path& path) const {
    return false;
}

void xc::config::parse_config() {
    if(file) {
        auto keyboard_res = file->get_qualified_as<bool>("Project.keyboard");
        if(keyboard_res) {
            has_keyboard = *keyboard_res;
        }
        auto debug_res = file->get_qualified_as<bool>("Project.debug");
        if(debug_res) {
            debug = *debug_res;
        } else {
            std::cout<<"No debug?";
        }
        
        auto asset_res = file->get_qualified_as<std::string>("Project.assets");
        if(asset_res) {
            asset_dir = *asset_res;
        }

        auto disp_table = file->get_table("Display");
        if(disp_table) {
            auto fullscreen_res = disp_table->get_as<bool>("fullscreen");
            if(fullscreen_res) {
                is_fullscreen = *fullscreen_res;
            }

            std::string temp_font;
            if(asset_res) {
                auto fnt_res = disp_table->get_as<std::string>("font");
                if (fnt_res) {
                    temp_font = *fnt_res;
                }
            }
            if(!temp_font.empty()) {
                font = asset_dir;
                font.append(asset_folders::FONT_DIR);
                font.append(temp_font);
            }
        }

        if(debug) {
            std::cout<<"Loaded configuration is"<<std::endl;
            std::cout<<"  Fullscreen: "<<is_fullscreen<<std::endl;
            std::cout<<"  Keyboard: "<<has_keyboard<<std::endl;
            std::cout<<"  Assets directory: "<<asset_dir<<std::endl;
            if(!font.empty()) {
                std::cout<<"  Default font: "<<font<<std::endl;
            }
        }
    } else {
        std::cout<<"File was invalid"<<std::endl;
    }
}

bool xc::config::fullscreen() const noexcept {
    return is_fullscreen;
}

bool xc::config::keyboard() const noexcept {
    return has_keyboard;
}

const std::filesystem::path& xc::config::asset_directory() const noexcept {
    return asset_dir;
}

std::filesystem::path xc::config::font_directory() const {
    std::filesystem::path result = asset_dir;
    result.append(asset_folders::FONT_DIR);
    return asset_dir;
}

const bool xc::config::has_default_font() const noexcept {
    return !font.empty();
}

const std::filesystem::path& xc::config::default_font() const noexcept {
    return font;
}

void xc::config::set_default_font(const std::filesystem::path& font) {
    this->font = font;
}
