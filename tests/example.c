#include "fungl.h"
#include <stdlib.h>

typedef struct Camera {
    vec3 position;       // Camera position
    vec3 target;         // Camera target it looks-at
    vec3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} Camera;

static Camera camera;

static void display_func(void) {
    gluClearScreenBuffers();
    
    mat4 matProj = perspective(TO_DEGREES(camera.fovy), 640.f / 480.f, 0.01, 1000.0);
    mat4 matView = look_at(camera.position, camera.target, camera.up);
    
    gluSetMatrixModelview(matView);    // Set internal modelview matrix (default shader)
    gluSetMatrixProjection(matProj);   // Set internal projection matrix (default shader)
    
    gluLoadDrawCube();
    
    // Draw internal render batch buffers (3D data)
    gluDrawRenderBatchActive();
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
    
    // Initialize viewport and internal projection/modelview matrices
    gluViewport(0, 0, 640, 480);
    gluMatrixMode(GLU_PROJECTION);                        // Switch to PROJECTION matrix
    gluLoadIdentity();                                   // Reset current matrix (PROJECTION)
    gluOrtho(0, 640, 480, 0, 0.0f, 1.0f); // Orthographic projection with top-left corner at (0,0)
    gluMatrixMode(GLU_MODELVIEW);                         // Switch back to MODELVIEW matrix
    gluLoadIdentity();                                   // Reset current matrix (MODELVIEW)
    
    gluClearColor(245, 245, 245, 255);                   // Define clear color
    gluEnableDepthTest();                                // Enable DEPTH_TEST for 3D
    
    camera = (Camera) {
        .position = Vec3(5.0f, 5.0f, 5.0f),
        .target = Vec3(0.0f, 0.0f, 0.0f),
        .up = Vec3(0.0f, 1.0f, 0.0f)
    };
    
    glutMainLoop();
    return 0;
}
