/* glu.h --  https://github.com/takeiteasy/fungl
 
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

#if !defined(glu_h) && !defined(FUNGL_NO_GLU)
#define glu_h
#if defined(__cplusplus)
extern "C" {
#endif
#if FUNGL_VERSION < GL_VERSION_3_2
#error "glu.h requires FUNGL_VERSION >= GL_VERSION_3_2"
#endif
#include "gl.h"
#include "glm.h"

#if defined(__cplusplus)
}
#endif
#endif /* glu_h */
