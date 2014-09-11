
#include "OpenGLRenderer.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

PFNGLGENBUFFERSPROC glGenBuffers = nullptr;

PFNGLBINDBUFFERPROC glBindBuffer = nullptr;

PFNGLBUFFERDATAPROC glBufferData = nullptr;

PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;

PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;

// New Shader Related Functions
PFNGLCREATESHADERPROC				glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC				glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC				glCompileShader = nullptr;
PFNGLGETSHADERIVPROC				glGetShaderiv = nullptr;
PFNGLATTACHSHADERPROC				glAttachShader = nullptr;
PFNGLCREATEPROGRAMPROC				glCreateProgram = nullptr;
PFNGLLINKPROGRAMPROC				glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC				glGetProgramiv = nullptr;
PFNGLUSEPROGRAMPROC					glUseProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation = nullptr;
PFNGLUNIFORM1FPROC					glUniform1f = nullptr;
PFNGLUNIFORM1IPROC					glUniform1i = nullptr;
PFNGLACTIVETEXTUREPROC				glActiveTexture = nullptr;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog = nullptr; 
PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog = nullptr;
PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray = nullptr;
PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation = nullptr;
PFNGLUNIFORM3FPROC                  glUniform3f = nullptr;
PFNGLUNIFORM3FVPROC					glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC					glUniform4fv = nullptr;
PFNGLUNIFORM4FPROC					glUniform4f = nullptr;
PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC            glBindFramebuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D = nullptr;

