#include "fungl.h"

static void display_func(void) {
    
}

static void reshape_func(int w, int h) {
    
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glInit();
    gluInit(640, 480);
    glutDisplayFunc(display_func);
    glutReshapeFunc(reshape_func);
    glutCreateWindow("test");
    glutMainLoop();
    return 0;
}
