#ifndef AIRSPEED_TAPE_H
#define AIRSPEED_TAPE_H

#include <memory>
#include <string>

#include "../glpp/program.h"
#include "../glpp/wrappers.h"
#include "base_symbol.h"

namespace sym {

//! Symbol for airspeed
class airspeed : public base_symbol {
  public:
    virtual void init() override;
    virtual void render() override;

  private:
    std::shared_ptr<glp::program> shader;
    std::unique_ptr<glp::buffer_obj> mesh;

    size_t size;
};
} // namespace sym

#endif
