#ifndef SYMBOLS_PONG_H
#define SYMBOLS_PONG_H

#include <memory>
#include <utility>
#include <random>

#include "../glpp/program.h"
#include "../glpp/wrappers.h"
#include "base_symbol.h"

namespace sym {
enum class player {
    one, two
};

class pong: public base_symbol {
  public:
    void init() override;
    void render() override;
    
    void reset();
    // From -1.0 to 1.0
    void set_paddle_position(player index, double height);
    
    void set_autoplay(player p, bool on);

    bool check_if_lost(player p);
    bool check_if_won(player p);
   private:
    void init_shaders();
    void update_ai(player p);
    void update_ball();

    struct paddle_state {
        std::pair<GLfloat, GLfloat> position;
        std::unique_ptr<glp::buffer_obj> mesh;
        size_t mesh_size;
        bool auto_play = true;
        GLfloat radius = 0.1;
        GLfloat width = 0.01;
        size_t score;
    };

    size_t rounds = 0;

    paddle_state& state(player p);

    paddle_state p1;
    paddle_state p2;

    std::pair<GLfloat, GLfloat> ball;
    std::pair<GLfloat, GLfloat> ball_velocity;

    std::default_random_engine eng;
    
    std::shared_ptr<glp::program> shader;
    std::unique_ptr<glp::buffer_obj> ball_mesh;
    size_t ball_size;
};
} // namespace sym

#endif

