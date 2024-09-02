/* glut.c -- http://www.github.com/takeiteasy/fungl
 
 The MIT License (MIT)

 Copyright (c) 2022 George Watson

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "glut.h"
#define GLFW_INCLUDE_GLU
#ifndef _WIN32
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

typedef void(*glut_display_callback)(void);
typedef void(*glut_reshape_callback)(int, int);
typedef void(*glut_keyboard_callback)(unsigned char, int, int);
typedef void(*glut_special_callback)(int, int, int);
typedef void(*glut_mouse_callback)(int, int, int, int);
typedef void(*glut_motion_callback)(int, int);
typedef void(*glut_passive_motion_callback)(int, int);

static struct {
    glut_display_callback display_callback;
    glut_reshape_callback reshape_callback;
    glut_keyboard_callback keyboard_callback;
    glut_special_callback special_callback;
    glut_mouse_callback mouse_callback;
    glut_motion_callback motion_callback;
    glut_passive_motion_callback passive_motion_callback;
    GLFWwindow *window;
    int initialised;
    unsigned int mode;
    int window_width;
    int window_height;
    int window_position_x;
    int window_position_y;
    int cursor_x;
    int cursor_y;
    int mouse_is_down;
    int modifier;
} glfw = {
    .window = NULL,
    .initialised = 0,
    .mode = GLUT_RGB | GLUT_SINGLE,
    .window_width = 640,
    .window_height = 480,
    .window_position_x = 0,
    .window_position_y = 0,
    .cursor_x = 0,
    .cursor_y = 0,
    .mouse_is_down = 0,
    .modifier = 0
};

void glutInit(int *argcp, char **argv) {
    if (!glfw.initialised)
        glfw.initialised = glfwInit();
}

void glutInitDisplayMode(unsigned int mode) {
    if (mode & GLUT_3_2_CORE_PROFILE) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }
    
    if (mode & GLUT_RGBA) {
        glfwWindowHint(GLFW_RED_BITS,  8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS,8);
        glfwWindowHint(GLFW_ALPHA_BITS,8);
    }
    
    if (mode & GLUT_DEPTH)
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
    
    if (mode & GLUT_DOUBLE)
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    
    if (mode & GLUT_MULTISAMPLE)
        glfwWindowHint(GLFW_SAMPLES, 8);
    
    glfw.mode = mode;
}

void glutInitDisplayString(const char *string) {
    // TODO
}

void glutInitWindowPosition(int x, int y) {
    if (!glfw.initialised) {
        glfw.window_position_x = x;
        glfw.window_position_y = y;
    }
}

void glutInitWindowSize(int width, int height) {
    if (!glfw.initialised) {
        glfw.window_width = width;
        glfw.window_height = height;
    }
}

void glutMainLoop(void) {
    int width_window, height_window;
    glfwGetWindowSize(glfw.window, &width_window, &height_window);
    glfw.reshape_callback(width_window, height_window);
    
    // TODO: Properly mimic glut's loop
    while (!glfwWindowShouldClose(glfw.window)) {
        glfw.display_callback();
        glfwSwapBuffers(glfw.window);
        glfwPollEvents();
    }
}


static void glutMouseButtonCallback(GLFWwindow *window, int button, int action, int mod) {
    glfw.modifier = mod;
    switch(button) {
        default:
        case GLFW_MOUSE_BUTTON_1:
            button = GLUT_LEFT_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_2:
            button = GLUT_RIGHT_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_3:
            button = GLUT_MIDDLE_BUTTON;
            break;
    }
    switch(action) {
        case GLFW_PRESS:
            action = GLUT_DOWN;
            glfw.mouse_is_down = 1;
            break;
        default:
            action = GLUT_UP;
            glfw.mouse_is_down = 0;
            break;
    }
    glfw.mouse_callback(button, action, glfw.cursor_x, glfw.cursor_y);
}

static void glutCursorPosCallback(GLFWwindow *window, double x, double y) {
    glfw.cursor_x = x;
    glfw.cursor_y = y;
    if (glfw.mouse_is_down)
        glfw.motion_callback(glfw.cursor_x, glfw.cursor_y);
    else
        glfw.passive_motion_callback(glfw.cursor_x, glfw.cursor_y);
}

static void glutCharModsCallback(GLFWwindow *window, unsigned int code, int mod) {
    switch(code) {
        default:
            glfw.keyboard_callback((unsigned char)code, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F1:
            glfw.special_callback(GLUT_KEY_F1, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F2:
            glfw.special_callback(GLUT_KEY_F2, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F3:
            glfw.special_callback(GLUT_KEY_F3, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F4:
            glfw.special_callback(GLUT_KEY_F4, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F5:
            glfw.special_callback(GLUT_KEY_F5, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F6:
            glfw.special_callback(GLUT_KEY_F6, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F7:
            glfw.special_callback(GLUT_KEY_F7, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F8:
            glfw.special_callback(GLUT_KEY_F8, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F9:
            glfw.special_callback(GLUT_KEY_F9, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F10:
            glfw.special_callback(GLUT_KEY_F10, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F11:
            glfw.special_callback(GLUT_KEY_F11, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_F12:
            glfw.special_callback(GLUT_KEY_F12, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_LEFT:
            glfw.special_callback(GLUT_KEY_LEFT, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_UP:
            glfw.special_callback(GLUT_KEY_UP, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_RIGHT:
            glfw.special_callback(GLUT_KEY_RIGHT, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_DOWN:
            glfw.special_callback(GLUT_KEY_DOWN, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_PAGE_UP:
            glfw.special_callback(GLUT_KEY_PAGE_UP, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_PAGE_DOWN:
            glfw.special_callback(GLUT_KEY_PAGE_DOWN, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_HOME:
            glfw.special_callback(GLUT_KEY_HOME, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_END:
            glfw.special_callback(GLUT_KEY_END, glfw.cursor_x, glfw.cursor_y);
            break;
        case GLFW_KEY_INSERT:
            glfw.special_callback(GLUT_KEY_INSERT, glfw.cursor_x, glfw.cursor_y);
            break;
    }
}

static void glutKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mod) {
    glfw.modifier = mod;
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch(key) {
            default:
                break;
            case 67:
                if(mod & GLFW_MOD_CONTROL)
                    glfw.keyboard_callback((char)3, glfw.cursor_x, glfw.cursor_y);
                break;
            case GLFW_KEY_BACKSPACE:
                glfw.keyboard_callback((char)127, glfw.cursor_x, glfw.cursor_y);
                break;
            case GLFW_KEY_ESCAPE:
                glfw.keyboard_callback((char)27, glfw.cursor_x, glfw.cursor_y);
                break;
        }
    }
}

static void apply(double *_x, double *_x_surplus, int right, int left) {
    double x = *_x;
    double x_surplus = *_x_surplus;
    x += x_surplus;
    x_surplus = x-round(x);
    x = round(x-x_surplus);
    int dir = x>0 ? right : left;
    x = fabs(x);
    while(x != 0) {
        glfw.mouse_callback(dir, GLUT_DOWN, glfw.cursor_x, glfw.cursor_y);
        x--;
    }
}

static void glutScrollCallback(GLFWwindow *window, double x, double y) {
    static double x_surplus = 0;
    static double y_surplus = 0;
    apply(&x, &x_surplus, GLUT_WHEEL_RIGHT, GLUT_WHEEL_LEFT);
    apply(&y, &y_surplus, GLUT_WHEEL_UP, GLUT_WHEEL_DOWN);
}

static void glutWindowSizeCallback(GLFWwindow * window, int w, int h) {
    glfw.reshape_callback(w, h);
}

int glutCreateWindow(const char *name) {
    if (glfw.window != NULL)
        abort(); // TODO: Support multiple windows
    
    if (!(glfw.window = glfwCreateWindow(glfw.window_width, glfw.window_height, name, NULL, NULL)))
        abort();
    
    glfwMakeContextCurrent(glfw.window);
    if (glfw.window_position_x != 0 || glfw.window_position_y != 0)
        glfwSetWindowPos(glfw.window, glfw.window_position_x, glfw.window_position_y);
    
    glfwSetMouseButtonCallback(glfw.window, NULL);
    glfwSetCursorPosCallback(glfw.window, NULL);
    glfwSetCharModsCallback(glfw.window, NULL);
    glfwSetKeyCallback(glfw.window, NULL);
    glfwSetScrollCallback(glfw.window, NULL);
    glfwSetWindowSizeCallback(glfw.window, NULL);
    return 1;
}

int glutCreateSubWindow(int win, int x, int y, int width, int height) {
    return -1;
}

void glutDestroyWindow(int win) {
    // ...
}

void glutPostRedisplay(void) {
    // ...
}

void glutSwapBuffers(void) {
    glfwSwapBuffers(glfw.window);
}

int glutGetWindow(void) {
    return -1;
}

void glutSetWindow(int win) {
    // ...
}

void glutSetWindowTitle(const char *title) {
    // ...
}

void glutSetIconTitle(const char *title) {
    // ...
}

void glutPositionWindow(int x, int y) {
    // ...
}

void glutReshapeWindow(int width, int height) {
    // ...
}

void glutPopWindow(void) {
    // ...
}

void glutPushWindow(void) {
    // ...
}

void glutIconifyWindow(void) {
    // ...
}

void glutShowWindow(void) {
    // ...
}

void glutHideWindow(void) {
    // ...
}

void glutFullScreen(void) {
    // ...
}

void glutSetCursor(int cursor) {
    // ...
}

void glutWarpPointer(int x, int y) {
    // ...
}

void glutEstablishOverlay(void) {
    // ...
}

void glutRemoveOverlay(void) {
    // ...
}

void glutUseLayer(GLenum layer) {
    // ...
}

void glutPostOverlayRedisplay(void) {
    // ...
}

void glutPostWindowOverlayRedisplay(int win) {
    // ...
}

void glutShowOverlay(void) {
    // ...
}

void glutHideOverlay(void) {
    // ...
}

int glutCreateMenu(void (__cdecl *func)(int)) {
    return -1;
}

void glutDestroyMenu(int menu) {
    // ...
}

int glutGetMenu(void) {
    return -1;
}

void glutSetMenu(int menu) {
    // ...
}

void glutAddMenuEntry(const char *label, int value) {
    // ...
}

void glutAddSubMenu(const char *label, int submenu) {
    // ...
}

void glutChangeToMenuEntry(int item, const char *label, int value) {
    // ...
}

void glutChangeToSubMenu(int item, const char *label, int submenu) {
    // ...
}

void glutRemoveMenuItem(int item) {
    // ...
}

void glutAttachMenu(int button) {
    // ...
}

void glutDetachMenu(int button) {
    // ...
}

void glutDisplayFunc(void (*func)(void)) {
    glfw.display_callback = func;
}

void glutReshapeFunc(void (*func)(int width, int height)) {
    glfw.reshape_callback = func;
}

void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y)) {
    glfw.keyboard_callback = func;
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y)) {
    glfw.mouse_callback = func;
}

void glutMotionFunc(void (*func)(int x, int y)) {
    glfw.motion_callback = func;
}

void glutPassiveMotionFunc(void (*func)(int x, int y)) {
    glfw.passive_motion_callback = func;
}

void glutVisibilityFunc(void (*func)(int state)) {
    // ...
}

void glutIdleFunc(void (*func)(void)) {
    // ...
}

void glutTimerFunc(unsigned int millis, void (*func)(int value), int value) {
    // ...
}

void glutMenuStateFunc(void (*func)(int state)) {
    // ...
}

void glutSpecialFunc(void (*func)(int key, int x, int y)) {
    glfw.special_callback = func;
}

void glutSpaceballMotionFunc(void (__cdecl *func)(int x, int y, int z)){
    // ...
}

void glutSpaceballRotateFunc(void (__cdecl *func)(int x, int y, int z)){
    // ...
}

void glutSpaceballButtonFunc(void (__cdecl *func)(int button, int state)){
    // ...
}

void glutButtonBoxFunc(void (__cdecl *func)(int button, int state)){
    // ...
}

void glutDialsFunc(void (__cdecl *func)(int dial, int value)){
    // ...
}

void glutTabletMotionFunc(void (__cdecl *func)(int x, int y)){
    // ...
}

void glutTabletButtonFunc(void (__cdecl *func)(int button, int state, int x, int y)){
    // ...
}

void glutMenuStatusFunc(void (__cdecl *func)(int status, int x, int y)){
    // ...
}

void glutOverlayDisplayFunc(void (__cdecl *func)(void)){
    // ...
}

void glutWindowStatusFunc(void (__cdecl *func)(int state)){
    // ...
}

void glutKeyboardUpFunc(void (__cdecl *func)(unsigned char key, int x, int y)){
    // ...
}

void glutSpecialUpFunc(void (__cdecl *func)(int key, int x, int y)){
    // ...
}

void glutJoystickFunc(void (__cdecl *func)(unsigned int buttonMask, int x, int y, int z), int pollInterval) {
    // ...
}

void glutSetColor(int cell, GLfloat red, GLfloat green, GLfloat blue){
    // ...
}

GLfloat glutGetColor(int ndx, int component){
    return -1.f;
}

void glutCopyColormap(int win){
    // ...
}

int glutGet(GLenum type){
    return -1;
}

int glutDeviceGet(GLenum type){
    return -1;
}

int glutExtensionSupported(const char *name){
    return -1;
}


int glutGetModifiers(void) {
    int mod = 0;
    mod |= glfw.modifier & GLFW_MOD_SHIFT ? GLUT_ACTIVE_SHIFT : 0;
    mod |= glfw.modifier & GLFW_MOD_ALT ? GLUT_ACTIVE_ALT : 0;
    mod |= glfw.modifier & GLFW_MOD_CONTROL ? GLUT_ACTIVE_CTRL : 0;
    mod |= glfw.modifier & GLFW_MOD_SUPER ? GLUT_ACTIVE_COMMAND : 0;
    return mod;
}

int glutLayerGet(GLenum type) {
    return -1;
}

void glutBitmapCharacter(void *font, int character){
    // ...
}

int glutBitmapWidth(void *font, int character){
    return -1;
}

void glutStrokeCharacter(void *font, int character){
    // ...
}

int glutStrokeWidth(void *font, int character){
    return -1;
}

int glutBitmapLength(void *font, const unsigned char *string){
    return -1;
}

int glutStrokeLength(void *font, const unsigned char *string){
    return -1;
}

float glutStrokeWidthf(void *font, int character){
    return -1.f;
}

float glutStrokeLengthf(void *font, const unsigned char *string){
    return -1.f;
}

void glutWireSphere(GLdouble radius, GLint slices, GLint stacks){
    // ...
}

void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks){
    // ...
}

void glutWireCone(GLdouble base, GLdouble height, GLint slices, GLint stacks){
    // ...
}

void glutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks){
    // ...
}

void glutWireCube(GLdouble size){
    // ...
}

void glutSolidCube(GLdouble size){
    // ...
}

void glutWireTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings){
    // ...
}

void glutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings){
    // ...
}

void glutWireDodecahedron(void){
    // ...
}

void glutSolidDodecahedron(void){
    // ...
}

void glutWireTeapot(GLdouble size){
    // ...
}

void glutSolidTeapot(GLdouble size){
    // ...
}

void glutWireOctahedron(void){
    // ...
}

void glutSolidOctahedron(void){
    // ...
}

void glutWireTetrahedron(void){
    // ...
}

void glutSolidTetrahedron(void){
    // ...
}

void glutWireIcosahedron(void){
    // ...
}

void glutSolidIcosahedron(void){
    // ...
}

int glutVideoResizeGet(GLenum param){
    return -1;
}

void glutSetupVideoResizing(void){
    // ...
}

void glutStopVideoResizing(void){
    // ...
}

void glutVideoResize(int x, int y, int width, int height){
    // ...
}

void glutVideoPan(int x, int y, int width, int height){
    // ...
}

void glutReportErrors(void){
    // ...
}

void glutIgnoreKeyRepeat(int ignore){
    // ...
}

void glutSetKeyRepeat(int repeatMode){
    // ...
}

void glutForceJoystickFunc(void){
    // ...
}

void glutGameModeString(const char *string){
    // ...
}

int glutEnterGameMode(void){
    return -1;
}

void glutLeaveGameMode(void){
    // ...
}

int glutGameModeGet(GLenum mode){
    return -1;
}
