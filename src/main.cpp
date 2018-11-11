#include <GL/glut.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <memory>

#include "glpp/display_list.h"

std::unique_ptr<glp::display_list> line;
size_t frame_n=0;

void keyboard_event(unsigned char key, int x, int y) {

}

void reshape(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,h,0);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cout<<"Frame "<<frame_n<<": GL Error "<<err<<std::endl;
    }
    frame_n++;
    glutPostRedisplay();
}

void renderString(int x, int y, const std::string& s) {
    glRasterPos2f(x, y);
    for(const char &c: s) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glColor3f(0.0f, 1.0f, 0.0f);
    renderString(50.0f, 50.0f, "Hello world");
    if(line) {
        line->call_list();
    }
    glutSwapBuffers();
}

void init() {
    int mode = GLUT_RGB|GLUT_SINGLE;
    glutInitDisplayMode(mode);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard_event);
    glutIdleFunc(idle);
    line = std::make_unique<glp::display_list>();
    auto lock = line->new_list(0);
    glBegin(GL_LINE_STRIP);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(50.0f, 50.0f);
        glVertex2f(0.0f, 500.0f);
    glEnd();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow(argv[0]);
    
    init();
    std::cout<<"Starting update loop"<<std::endl; 
    glutMainLoop();
    return 0;
}
