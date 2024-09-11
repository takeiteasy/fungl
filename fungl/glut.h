/* glut.h --  https://github.com/takeiteasy/fungl
 
 fungl puts the fun back into OpenGL
 
 Copyright (C) 2024  George Watson

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef glut_h
#define glut_h
#if defined(__cplusplus)
extern "C" {
#endif

#include "gl.h"

/* Display mode bit masks. */
#define GLUT_RGB                           0
#define GLUT_RGBA                          GLUT_ALPHA
#define GLUT_INDEX                         1
#define GLUT_SINGLE                        0
#define GLUT_DOUBLE                        2
#define GLUT_ACCUM                         4 // UNIMPLEMENTED
#define GLUT_ALPHA                         8
#define GLUT_DEPTH                         16 // TODO
#define GLUT_STENCIL                       32 // TODO
#define GLUT_MULTISAMPLE                   128
#define GLUT_STEREO                        256 // UNIMPLEMENTED
#define GLUT_LUMINANCE                     512 // UNIMPLEMENTED
#define GLUT_3_2_CORE_PROFILE              1024
/* Mouse buttons. */
#define GLUT_LEFT_BUTTON                   0
#define GLUT_MIDDLE_BUTTON                 1
#define GLUT_RIGHT_BUTTON                  2
#define GLUT_WHEEL_UP                      3
#define GLUT_WHEEL_DOWN                    4
#define GLUT_WHEEL_LEFT                    5
#define GLUT_WHEEL_RIGHT                   6
/* Mouse button  state. */
#define GLUT_DOWN                          0
#define GLUT_UP                            1
/* function keys */
#define GLUT_KEY_F1                        1
#define GLUT_KEY_F2                        2
#define GLUT_KEY_F3                        3
#define GLUT_KEY_F4                        4
#define GLUT_KEY_F5                        5
#define GLUT_KEY_F6                        6
#define GLUT_KEY_F7                        7
#define GLUT_KEY_F8                        8
#define GLUT_KEY_F9                        9
#define GLUT_KEY_F10                       10
#define GLUT_KEY_F11                       11
#define GLUT_KEY_F12                       12
/* directional keys */
#define GLUT_KEY_LEFT                      100
#define GLUT_KEY_UP                        101
#define GLUT_KEY_RIGHT                     102
#define GLUT_KEY_DOWN                      103
#define GLUT_KEY_PAGE_UP                   104
#define GLUT_KEY_PAGE_DOWN                 105
#define GLUT_KEY_HOME                      106
#define GLUT_KEY_END                       107
#define GLUT_KEY_INSERT                    108
#define GLUT_LEFT                          0
#define GLUT_ENTERED                       1
#define GLUT_ACTIVE_SHIFT                  1
#define GLUT_ACTIVE_CTRL                   2
#define GLUT_ACTIVE_ALT                    4
#define GLUT_ACTIVE_COMMAND                8
/* glutSetCursor parameters. */
/* Basic arrows. */
#define GLUT_CURSOR_RIGHT_ARROW            0
#define GLUT_CURSOR_LEFT_ARROW             1
/* Symbolic cursor shapes. */
#define GLUT_CURSOR_INFO                   2
#define GLUT_CURSOR_DESTROY                3
#define GLUT_CURSOR_HELP                   4
#define GLUT_CURSOR_CYCLE                  5
#define GLUT_CURSOR_SPRAY                  6
#define GLUT_CURSOR_WAIT                   7
#define GLUT_CURSOR_TEXT                   8
#define GLUT_CURSOR_CROSSHAIR              9
/* Directional cursors. */
#define GLUT_CURSOR_UP_DOWN                10
#define GLUT_CURSOR_LEFT_RIGHT             11
/* Sizing cursors. */
#define GLUT_CURSOR_TOP_SIDE               12
#define GLUT_CURSOR_BOTTOM_SIDE            13
#define GLUT_CURSOR_LEFT_SIDE              14
#define GLUT_CURSOR_RIGHT_SIDE             15
#define GLUT_CURSOR_TOP_LEFT_CORNER        16
#define GLUT_CURSOR_TOP_RIGHT_CORNER       17
#define GLUT_CURSOR_BOTTOM_RIGHT_CORNER    18
#define GLUT_CURSOR_BOTTOM_LEFT_CORNER     19
/* Inherit from parent window. */
#define GLUT_CURSOR_INHERIT                100
/* Blank cursor. */
#define GLUT_CURSOR_NONE                   101
/* Fullscreen crosshair (if available). */
#define GLUT_CURSOR_FULL_CROSSHAIR         102

void glutInit(int *argcp, char **argv);
void glutInitDisplayMode(unsigned int mode);
void glutInitDisplayString(const char *string);
void glutInitWindowPosition(int x, int y);
void glutInitWindowSize(int width, int height);
void glutMainLoop(void);

/* GLUT window sub-API. */
int glutCreateWindow(const char *title);
int glutCreateSubWindow(int win, int x, int y, int width, int height);
void glutDestroyWindow(int win);
void glutPostRedisplay(void);
void glutPostWindowRedisplay(int win);
void glutSwapBuffers(void);
int glutGetWindow(void);
void glutSetWindow(int win);
void glutSetWindowTitle(const char *title);
void glutSetIconTitle(const char *title);
void glutPositionWindow(int x, int y);
void glutReshapeWindow(int width, int height);
void glutPopWindow(void);
void glutPushWindow(void);
void glutIconifyWindow(void);
void glutShowWindow(void);
void glutHideWindow(void);
void glutFullScreen(void);
void glutSetCursor(int cursor);
void glutWarpPointer(int x, int y);

/* GLUT overlay sub-API. */
void glutEstablishOverlay(void);
void glutRemoveOverlay(void);
void glutUseLayer(GLenum layer);
void glutPostOverlayRedisplay(void);
void glutPostWindowOverlayRedisplay(int win);
void glutShowOverlay(void);
void glutHideOverlay(void);

/* GLUT menu sub-API. */
int glutCreateMenu(void (__cdecl *func)(int));
void glutDestroyMenu(int menu);
int glutGetMenu(void);
void glutSetMenu(int menu);
void glutAddMenuEntry(const char *label, int value);
void glutAddSubMenu(const char *label, int submenu);
void glutChangeToMenuEntry(int item, const char *label, int value);
void glutChangeToSubMenu(int item, const char *label, int submenu);
void glutRemoveMenuItem(int item);
void glutAttachMenu(int button);
void glutDetachMenu(int button);

/* GLUT window callback sub-API. */
void glutDisplayFunc(void (__cdecl *func)(void));
void glutReshapeFunc(void (__cdecl *func)(int width, int height));
void glutKeyboardFunc(void (__cdecl *func)(unsigned char key, int x, int y));
void glutMouseFunc(void (__cdecl *func)(int button, int state, int x, int y));
void glutMotionFunc(void (__cdecl *func)(int x, int y));
void glutPassiveMotionFunc(void (__cdecl *func)(int x, int y));
void glutEntryFunc(void (__cdecl *func)(int state));
void glutVisibilityFunc(void (__cdecl *func)(int state));
void glutIdleFunc(void (__cdecl *func)(void));
void glutTimerFunc(unsigned int millis, void (__cdecl *func)(int value), int value);
void glutMenuStateFunc(void (__cdecl *func)(int state));
void glutSpecialFunc(void (__cdecl *func)(int key, int x, int y));
void glutSpaceballMotionFunc(void (__cdecl *func)(int x, int y, int z));
void glutSpaceballRotateFunc(void (__cdecl *func)(int x, int y, int z));
void glutSpaceballButtonFunc(void (__cdecl *func)(int button, int state));
void glutButtonBoxFunc(void (__cdecl *func)(int button, int state));
void glutDialsFunc(void (__cdecl *func)(int dial, int value));
void glutTabletMotionFunc(void (__cdecl *func)(int x, int y));
void glutTabletButtonFunc(void (__cdecl *func)(int button, int state, int x, int y));
void glutMenuStatusFunc(void (__cdecl *func)(int status, int x, int y));
void glutOverlayDisplayFunc(void (__cdecl *func)(void));
void glutWindowStatusFunc(void (__cdecl *func)(int state));
void glutKeyboardUpFunc(void (__cdecl *func)(unsigned char key, int x, int y));
void glutSpecialUpFunc(void (__cdecl *func)(int key, int x, int y));
void glutJoystickFunc(void (__cdecl *func)(unsigned int buttonMask, int x, int y, int z), int pollInterval);

/* GLUT color index sub-API. */
void glutSetColor(int cell, GLfloat red, GLfloat green, GLfloat blue);
GLfloat glutGetColor(int ndx, int component);
void glutCopyColormap(int win);

/* GLUT state retrieval sub-API. */
int glutGet(GLenum type);
int glutDeviceGet(GLenum type);
/* GLUT extension support sub-API */
int glutExtensionSupported(const char *name);
int glutGetModifiers(void);
int glutLayerGet(GLenum type);

/* GLUT font sub-API */
void glutBitmapCharacter(void *font, int character);
int glutBitmapWidth(void *font, int character);
void glutStrokeCharacter(void *font, int character);
int glutStrokeWidth(void *font, int character);
int glutBitmapLength(void *font, const unsigned char *string);
int glutStrokeLength(void *font, const unsigned char *string);
float glutStrokeWidthf(void *font, int character);
float glutStrokeLengthf(void *font, const unsigned char *string);

/* GLUT pre-built models sub-API */
void glutWireSphere(GLdouble radius, GLint slices, GLint stacks);
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);
void glutWireCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
void glutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
void glutWireCube(GLdouble size);
void glutSolidCube(GLdouble size);
void glutWireTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
void glutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
void glutWireDodecahedron(void);
void glutSolidDodecahedron(void);
void glutWireTeapot(GLdouble size);
void glutSolidTeapot(GLdouble size);
void glutWireOctahedron(void);
void glutSolidOctahedron(void);
void glutWireTetrahedron(void);
void glutSolidTetrahedron(void);
void glutWireIcosahedron(void);
void glutSolidIcosahedron(void);

/* GLUT video resize sub-API. */
int glutVideoResizeGet(GLenum param);
void glutSetupVideoResizing(void);
void glutStopVideoResizing(void);
void glutVideoResize(int x, int y, int width, int height);
void glutVideoPan(int x, int y, int width, int height);

/* GLUT debugging sub-API. */
void glutReportErrors(void);

/* GLUT device control sub-API. */
/* glutSetKeyRepeat modes. */
#define GLUT_KEY_REPEAT_OFF             0
#define GLUT_KEY_REPEAT_ON              1
#define GLUT_KEY_REPEAT_DEFAULT         2

/* Joystick button masks. */
#define GLUT_JOYSTICK_BUTTON_A          1
#define GLUT_JOYSTICK_BUTTON_B          2
#define GLUT_JOYSTICK_BUTTON_C          4
#define GLUT_JOYSTICK_BUTTON_D          8

void glutIgnoreKeyRepeat(int ignore);
void glutSetKeyRepeat(int repeatMode);
void glutForceJoystickFunc(void);

/* GLUT game mode sub-API. */
/* glutGameModeGet. */
#define GLUT_GAME_MODE_ACTIVE           ((GLenum) 0)
#define GLUT_GAME_MODE_POSSIBLE         ((GLenum) 1)
#define GLUT_GAME_MODE_WIDTH            ((GLenum) 2)
#define GLUT_GAME_MODE_HEIGHT           ((GLenum) 3)
#define GLUT_GAME_MODE_PIXEL_DEPTH      ((GLenum) 4)
#define GLUT_GAME_MODE_REFRESH_RATE     ((GLenum) 5)
#define GLUT_GAME_MODE_DISPLAY_CHANGED  ((GLenum) 6)

void glutGameModeString(const char *string);
int glutEnterGameMode(void);
void glutLeaveGameMode(void);
int glutGameModeGet(GLenum mode);

#if defined(__cplusplus)
}
#endif
#endif /* glut_h */
