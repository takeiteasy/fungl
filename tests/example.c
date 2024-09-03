#include "fungl.h"
#include <stdlib.h>

static void display_func(void) {
    
}

static void reshape_func(int w, int h) {
    
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutDisplayFunc(display_func);
    glutReshapeFunc(reshape_func);
    glutCreateWindow("test");
    if (glInit())
        abort();
    gluInit(640, 480);
    glutMainLoop();
    return 0;
}
