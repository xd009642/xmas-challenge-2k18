#include <GL/glew.h>
#include "application.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <filesystem>
#include <iostream>
#include <string>


namespace fs = std::filesystem;

//! Called each tick
void update();
//! Called on window resize
void resize(int w, int h);
//! Render the application
void render();
//! Keyboard event
void keyboard_event(unsigned char key, int x, int y);


std::size_t framecount;


void renderString(int x, int y, const std::string_view s) {
    if(s.empty()) {
        return;
    }
    glRasterPos2f(x, y);
    auto offset = 0;
    for(const char &c: s) {
        if(c == '\n' || c == '\r') {
            offset += 15;
            glRasterPos2f(x, y+offset);
        } else {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }
}

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
    glutIdleFunc(update);
    glutKeyboardFunc(keyboard_event);
    glutDisplayFunc(render);

    auto err = glewInit();
    if(GLEW_OK != err) {
        std::cout<<"No GLEW holding this together"<<std::endl;
    }

    if(!GLEW_VERSION_2_1) {
        std::cerr<<"Error: Graphics card does not support OpenGL 2.0"<<std::endl;
    }

    std::cout<<"OpenGL version "<<glGetString(GL_VERSION)<<std::endl;
    if(config.has_default_font()) {
        fonts.load(config.default_font());
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

void update() {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cout<<"Frame "<<framecount<<": GL Error "<<err<<std::endl;
    }
    framecount++;
    glutPostRedisplay();
}


void resize(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,h,0);
    glMatrixMode(GL_MODELVIEW);
}


void render() {
    glp::font_engine& fonts = glp::font_engine::instance();
    xc::command_interface& cmd = xc::application::instance().command();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    //glColor3f(0.0f, 1.0f, 0.0f);

    //renderString(50, 15, cmd.display());
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    if(fonts.current_font().valid()) {
        glEnable(GL_TEXTURE_2D);
        auto tex = fonts.current_font().bind();
        auto dims = fonts.current_font().dimensions();
        glBegin(GL_QUADS);
            glTexCoord2d(0, 0); glVertex2i(10, 10); 
            glTexCoord2d(0, 1); glVertex2i(10, 10 + dims.length[1]*2); 
            glTexCoord2d(1, 1); glVertex2i(10 + dims.length[0]*2,10 + dims.length[1]*2); 
            glTexCoord2d(1, 0); glVertex2i(10 + dims.length[0]*2, 10); 
        glEnd();
    }
    fonts.render_text("Hello", 50, 0, 1, 1);
    //renderString(50, height - 15, cmd.cmd_string());
    glutSwapBuffers();
}


void keyboard_event(unsigned char key, int x, int y) {
    xc::application::instance().command().push_char(key);
}

