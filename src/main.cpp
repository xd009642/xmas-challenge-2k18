#include <GL/glut.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <memory>

#include "application.h"


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    xc::application& app = xc::application::instance();
    app.init();
    app.start();
    return 0;
}
