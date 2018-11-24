#include <GL/glew.h>
#include "application.h"
#include "input/input.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <utility>

#include "symbols/heading_tape.h"

sym::heading_tape heading;

namespace fs = std::filesystem;

//! Called each tick
void update(int dt);
//! Called on window resize
void resize(int w, int h);
//! Render the application
void render();
//! Keyboard event
void keyboard_event(unsigned char key, int x, int y);
//! Event for special keys - function keys arrow keys etc
void special_key_event(int key, int x, int y);

std::size_t framecount;


xc::application& xc::application::instance() {
    static xc::application ret;
    return ret;
}

xc::application::application():
    // TODO bit naughty creating a singleton like this. Might want to change this
    fonts(glp::font_engine::instance()){
    framecount = 0;
}


void xc::application::init() {
    if(!config.read("Config.toml")) {
        std::cout << "Failed to parse config file"<<std::endl;
    }
    init_graphics();
    
    tpg.init();
    heading.init();

    init_fonts();

    ip::create();
    
    std::cout<<"Application initialised "<<glGetError()<<std::endl;
}

        
void xc::application::init_graphics() {
    glutInitWindowSize(500,500);
    window = glutCreateWindow("Xmas Challenge 2018");

    if(config.fullscreen()) {
        glutFullScreen();
    }
    
    int mode = GLUT_RGB|GLUT_SINGLE;
    glutInitDisplayMode(mode);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutReshapeFunc(resize);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(keyboard_event);
    glutSpecialFunc(special_key_event);
    glutDisplayFunc(render);

    auto err = glewInit();
    if(GLEW_OK != err) {
        std::cout<<"No GLEW holding this together"<<std::endl;
    }

    if(!GLEW_VERSION_2_1) {
        std::cerr<<"Error: Graphics card does not support OpenGL 2.0"<<std::endl;
    }
    std::cout<<"OpenGL version "<<glGetString(GL_VERSION)<<std::endl;
}


void xc::application::init_fonts() {
    glp::program font_shader;
    std::filesystem::path f = config.asset_directory();
    f.append(xc::asset_folders::SHADER_DIR);
    f.append("colour_font.f.glsl");
    
    font_shader.load_fragment(f);
    f = f.parent_path();
    f.append("ident.v.glsl");
    font_shader.load_vertex(f);
    font_shader.compile(); 

    if(font_shader.valid()) {
        fonts.set_program(font_shader);
    }

    if(config.has_default_font()) {
        fonts.load(config.default_font());
    } else {
        for(const auto& p: fs::directory_iterator(config.font_directory())) {
            if(p.path().extension() == ".ttf") {
                if(fonts.load(p)) { //TODO should save this back somewhere
                    std::cout<<"Load font "<<p.path().filename()<<std::endl;
                }
                break;
            }
        }
    }
}


void xc::application::start() {
    std::cout<<"Starting application"<<std::endl;
    glutMainLoop();
}

void xc::application::close() {
    glutDestroyWindow(window);
    exit(0);
}
        

std::vector<std::string> xc::application::get_available_fonts() const {
    return {};
}


xc::command_interface& xc::application::command() {
    return cmd;
}

xc::test_pattern_generator& xc::application::test_pattern() {
    return tpg;
}

void update(int) {
    bool had_error = false;
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cout<<"Frame "<<framecount<<": GL Error "<<err<<std::endl;
        had_error = true;
    }
    if(had_error) {
        std::abort();
    }
    framecount++;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void resize(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,h,0);
    glMatrixMode(GL_MODELVIEW);
}


void render() {
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int font_margin = -width + 25;

    glp::font_engine& fonts = glp::font_engine::instance();
    auto& tpg = xc::application::instance().test_pattern();
    xc::command_interface& cmd = xc::application::instance().command();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    tpg.render();

    heading.render();
    fonts.render_text(cmd.display(), font_margin, height-40, 1, 1);
    fonts.render_text(cmd.cmd_string(), font_margin, -(height-30), 1, 1);
    glutSwapBuffers();
}


void keyboard_event(unsigned char key, int x, int y) {
    if(key == 27) {
        xc::application::instance().close();
        return;
    }
    xc::application::instance().command().push_char(key);
}

void special_key_event(int key, int x, int y) {

}
