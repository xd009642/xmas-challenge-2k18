#include <GL/glew.h>
#include <GL/glut.h>
#include "application.h"


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    xc::application& app = xc::application::instance();
    app.init();
    app.start();
    return 0;
}
