#include <GL/glut.h>
#include <GL/gl.h>


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    int mode = GLUT_RGB|GLUT_SINGLE;
    glutInitDisplayMode(mode);
    glutInitWindowSize(500,500);
    glutCreateWindow(argv[0]);
    glutDisplayFunc([](){});
    glutMainLoop();
    return 0;
}
