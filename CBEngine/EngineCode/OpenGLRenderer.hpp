
#ifndef included_OpenGLRenderer
#define included_OpenGLRenderer
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

extern PFNGLGENBUFFERSPROC glGenBuffers;

extern PFNGLBINDBUFFERPROC glBindBuffer;

extern PFNGLBUFFERDATAPROC glBufferData;

extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

/*
glCreateShader
glShaderSource
glCompileShader
glGetShaderiv
glAttachShader
glCreateProgram
glLinkProgram
glGetProgramiv
glUseProgram
glGetUniformLocation
glUniform1f
glActiveTexture
*/

// New Shader Related Functions
extern PFNGLCREATESHADERPROC				glCreateShader;
extern PFNGLSHADERSOURCEPROC				glShaderSource;
extern PFNGLCOMPILESHADERPROC				glCompileShader;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLATTACHSHADERPROC				glAttachShader;
extern PFNGLCREATEPROGRAMPROC				glCreateProgram;
extern PFNGLLINKPROGRAMPROC					glLinkProgram;
extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
extern PFNGLUSEPROGRAMPROC					glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
extern PFNGLUNIFORM1FPROC					glUniform1f;
extern PFNGLUNIFORM1IPROC					glUniform1i;

extern PFNGLUNIFORM3FVPROC					glUniform3fv;
extern PFNGLACTIVETEXTUREPROC				glActiveTexture;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog; 
extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
extern PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv;
extern PFNGLUNIFORM3FPROC                   glUniform3f;
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray;
extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
extern PFNGLUNIFORM4FVPROC					glUniform4fv;
extern PFNGLUNIFORM4FPROC					glUniform4f;
extern PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC             glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;


#endif