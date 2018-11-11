#include "application.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <string>
#include <iostream>


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

xc::application& xc::application::instance() noexcept {
    static xc::application ret;
    return ret;
}

xc::application::application() noexcept {
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
}


void xc::application::start() {
    std::cout<<"Starting application"<<std::endl;
    glutMainLoop();
}

void xc::application::close() {
    glutDestroyWindow(window);
    exit(0);
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
    xc::command_interface& cmd = xc::application::instance().command();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glColor3f(0.0f, 1.0f, 0.0f);
    renderString(50, 15, cmd.display());
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    renderString(50, height - 15, cmd.cmd_string());
    glutSwapBuffers();
}


void keyboard_event(unsigned char key, int x, int y) {
    xc::application::instance().command().push_char(key);
}

