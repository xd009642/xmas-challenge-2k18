#ifndef HEADING_TAPE_H
#define HEADING_TAPE_H

#include <memory>
#include <string>

#include "base_symbol.h"
#include "../glpp/program.h"
#include "../glpp/wrappers.h"

namespace sym {

    //! Symbol for heading tape on top of display
    class heading_tape: public base_symbol {
    public:
        virtual void init() override;
        virtual void render() override;

    private:
        std::shared_ptr<glp::program> shader;
        std::unique_ptr<glp::buffer_obj> mesh;

        size_t size;
    };
}

#endif
