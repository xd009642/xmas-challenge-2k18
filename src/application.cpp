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


void renderString(int x, int y, const std::string& s) {
    glRasterPos2f(x, y);
    for(const char &c: s) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
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

    glutInitWindowSize(500,500);
    glutCreateWindow("Xmas Challenge 2018");

    int mode = GLUT_RGB|GLUT_SINGLE;
    glutInitDisplayMode(mode);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutReshapeFunc(resize);
    glutDisplayFunc(render);
    glutIdleFunc(update);
    glutKeyboardFunc(keyboard_event);
}


void xc::application::start() {
    glutMainLoop();
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glColor3f(0.0f, 1.0f, 0.0f);
    renderString(50.0f, 50.0f, "Hello world");
    glutSwapBuffers();
}


void keyboard_event(unsigned char key, int x, int y) {
}
