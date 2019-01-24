#include "pong.h"
#include "stdlib.h"
#include "time.h"
#include <iostream>
#include "../glpp/gl_includes.h"
#include "../glpp/types.h"
#include "mesh_util.h"


void sym::pong::reset() {
    p1.score = 0;
    p2.score = 0;
    ball.first = 0;
    ball.second = 0;
    ball_velocity.first =0; ball_velocity.second = 0;
    
    ball_velocity.first = (float)rand()/RAND_MAX-0.5f;
    ball_velocity.second = (float)rand()/RAND_MAX - 0.5f;
    if(std::abs(ball_velocity.first) < 0.4f) {
        ball_velocity.first = rand()%2? -0.4f:0.4f;
    }
    if(std::abs(ball_velocity.second) < 0.1f) {
        ball_velocity.second = rand()%2? -0.1f:0.1f;
    }

    p1.position.first = -0.9;
    p2.position.first = 0.9;
    p1.position.second = 0.0;
    p2.position.second = 0.0;
}

void sym::pong::init() {
    srand(time(0));
    reset();

    ball_mesh = std::make_unique<glp::buffer_obj>();
    if(ball_mesh) {
        std::vector<glp::point<GLfloat>> coords = {{0.0f, 0.0f}};
        glBindBuffer(GL_ARRAY_BUFFER, ball_mesh->id());
        auto ball_coords = sym::make_line_mesh(coords, 0.02f);
        glBufferData(GL_ARRAY_BUFFER,
                ball_coords.size()*sizeof(glp::point<GLfloat>),
                ball_coords.data(),
                GL_DYNAMIC_DRAW);
        ball_size = ball_coords.size()*2;
    }

    p1.mesh = std::make_unique<glp::buffer_obj>();
    if(p1.mesh) {
        std::vector<glp::point<GLfloat>> coords{{0, p1.radius}, {0, -p1.radius}};
        glBindBuffer(GL_ARRAY_BUFFER, p1.mesh->id());
        auto p1_coords = sym::make_line_mesh(coords, p1.width); 
        glBufferData(GL_ARRAY_BUFFER,
                p1_coords.size()*sizeof(glp::point<GLfloat>),
                p1_coords.data(),
                GL_DYNAMIC_DRAW);
        p1.mesh_size = p1_coords.size()*2;
    }
    p2.mesh = std::make_unique<glp::buffer_obj>();
    if(p2.mesh) {
        std::vector<glp::point<GLfloat>> coords;
        coords.emplace_back(0, p2.radius);
        coords.emplace_back(0, -p2.radius);
        glBindBuffer(GL_ARRAY_BUFFER, p2.mesh->id());
        auto p2_coords = sym::make_line_mesh(coords, p2.width); 
        glBufferData(GL_ARRAY_BUFFER,
                p2_coords.size()*sizeof(glp::point<GLfloat>),
                p2_coords.data(),
                GL_DYNAMIC_DRAW);
        p2.mesh_size = p2_coords.size() *2;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    init_shaders();
}

void sym::pong::init_shaders() {
    std::string vert = "#version 120\n"
                       "attribute vec2 coord;"
                       "uniform vec2 pos;\n"
                       "void main(void) {\n"
                       "    gl_Position = vec4(coord.xy+pos.xy, 0.0, 1.0);\n"
                       "}";

    std::string frag = "#version 120\n"
                       "void main(void) {\n"
                       "gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
                       "}";

    shader = std::make_shared<glp::program>();
    if(shader) {
        shader->load_vertex(vert);
        shader->load_fragment(frag);
        shader->compile();
    }
}

void sym::pong::render() {
    update_ball();
    update_ai(sym::player::one);
    update_ai(sym::player::two);
    if(p1.mesh && p2.mesh && ball_mesh && shader) {
        auto coord = shader->get_attrib("coord");
        shader->attach();
        GLint height = shader->get_uniform("pos");

        glEnableVertexAttribArray(coord);
        glUniform2f(height, ball.first, ball.second);
        glBindBuffer(GL_ARRAY_BUFFER, ball_mesh->id());
        
        glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, ball_size * 2);

        glUniform2f(height, p1.position.first, p1.position.second);
        glBindBuffer(GL_ARRAY_BUFFER, p1.mesh->id());
        
        glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, p1.mesh_size * 2);
        glUniform2f(height, p2.position.first, p2.position.second);
        glBindBuffer(GL_ARRAY_BUFFER, p2.mesh->id());
        
        glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, p2.mesh_size * 2);
        shader->detach();

    }
}

void sym::pong::update_ai(sym::player p) {
    auto& bob = state(p);
    if(bob.auto_play) {
        set_paddle_position(p, ball.second - bob.position.second );
    }
}   

void sym::pong::set_autoplay(sym::player index, bool on) {
    state(index).auto_play = on;
}


void sym::pong::set_paddle_position(sym::player p, double delta) {
    auto& pers = state(p);
    if(std::abs(delta) > 0.015f) {
        float scale = 0.015f/std::abs(delta);
        delta *= scale;
    }
    float proposed =  pers.position.second + delta;
    if(proposed - pers.radius < -1) {
        proposed = -1 + pers.radius;
    } else if(proposed + pers.radius > 1) {
        proposed = 1 - pers.radius;
    }
    pers.position.second = proposed;
}

sym::pong::paddle_state& sym::pong::state(sym::player p) {
    if(p == sym::player::one) {
        return p1;
    } else {
        return p2;
    }

}

void sym::pong::update_ball() {
    bool hit_paddle = false;
    if(ball.first <= -0.9f || ball.first >= 0.9f) {
        if((std::abs(ball.second - p1.position.second)*1.005f < p1.radius && ball.first<0) ||
                (std::abs(ball.second-p2.position.second)*1.005f < p2.radius) &&  ball.first>0) {
            ball_velocity.first *= -1.0f;
            hit_paddle = true;
            if((std::abs(ball.second - p1.position.second) < p1.radius*0.999999f && ball.first<0) ||
                    (std::abs(ball.second-p2.position.second) < p2.radius)*0.999999f &&  ball.first>0) {
                ball_velocity.second *= 1.5f;
            } else {
                ball_velocity.second *= 1.1f;
            }
        } else {
            if(ball.first < 0) {
                p2.score++;
            } else {
                p1.score++;
            }
            ball_velocity.second *=0.5f;
            ball.first = 0;
            ball.second = 0;
        }
    }else if(std::abs(ball.second) > 0.95f) {
        ball_velocity.second *= -1.0f;
    }
    if(std::abs(ball_velocity.first) < 0.1f) {
        ball_velocity.first = rand()%2? -0.1f:0.1f;
    }

    float vmag = std::sqrt(std::pow(ball_velocity.first-ball_velocity.second, 2));

    if(vmag > 1.0f) {
        ball_velocity.first /= vmag; 
        ball_velocity.second /= vmag;
    }

    ball.first += 0.027f*ball_velocity.first;
    ball.second += 0.027f*ball_velocity.second;

    if(ball.second >= 1.0f) {
        ball.second = 0.99f;
    } else if(ball.second <= -1.0f) {
        ball.second = -0.99f;
    }
    if(hit_paddle) {
        if(ball.first >= 0.9f) {
            ball.first = 0.87f;
        } else if(ball.first <= -0.9f) {
            ball.first = -0.87f;
        }
    }
}


bool sym::pong::check_if_lost(sym::player p) {
    if(p==sym::player::one) {
        if(state(sym::player::two).score >= 5) {
            return true;
        }
    } else {
        if(state(sym::player::one).score >= 5) {
            return true;
        }
    }
    return false;
}

bool sym::pong::check_if_won(sym::player p) {
    return state(p).score >= 5;
}
