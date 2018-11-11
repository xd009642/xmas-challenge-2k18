#include "config.h"

namespace fs = std::filesystem;

bool xc::config::read(const fs::path& path) {
    if(path.has_filename() && fs::exists(path)) {
        file = cpptoml::parse_file(path); 
            
    }
    return false;
}

bool xc::config::write(const fs::path& path) const {
    return false;
}
