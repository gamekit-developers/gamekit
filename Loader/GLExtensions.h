/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef	GLEXTENSIONS_H
#define GLEXTENSIONS_H

#ifdef WIN32

#define _WINSOCKAPI_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glext.h"
#include "wglext.h"

// Multi-texture
typedef void (APIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);

// VBO Extension Definitions, From glext.h
typedef void (APIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
//typedef void (APIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);


// Multi-texture
extern PFNGLACTIVETEXTUREPROC		glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC  glClientActiveTexture;

// VBO Extension Function Pointers
extern PFNGLGENBUFFERSPROC		glGenBuffers;					// VBO Name Generation Procedure
extern PFNGLBINDBUFFERPROC		glBindBuffer;					// VBO Bind Procedure
extern PFNGLBUFFERDATAPROC		glBufferData;					// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSPROC	glDeleteBuffers;				// VBO Deletion Procedure
extern PFNGLMAPBUFFERPROC		glMapBuffer;
extern PFNGLUNMAPBUFFERPROC		glUnmapBuffer;
extern PFNGLBUFFERSUBDATAPROC	glBufferSubData;

// ARB_texture_compression
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;

// EXT_framebuffer_object - http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
extern PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;

// wgl
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

bool initGLExtensions();

// VSYNC
bool InitVSync();
bool SetVSync(bool VSync);
bool GetVSync(bool* VSync);

#else
#include <GLUT/glut.h>
#endif

#endif