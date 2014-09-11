
#ifndef included_OpenGLShaderProgram
#define included_OpenGLShaderProgram
#pragma once

#include "EngineMacros.hpp"

class OpenGLShader;

class OpenGLShaderProgram {
public:

	~OpenGLShaderProgram();
	explicit OpenGLShaderProgram( const OpenGLShader & vertexShader, const OpenGLShader & fragmentShader );

	unsigned int	m_programID;

protected:


private:
	PREVENT_COPY_AND_ASSIGN( OpenGLShaderProgram );

	void attachAndLinkShaderProgram( const OpenGLShader & vertexShader, const OpenGLShader & fragmentShader );


};


#endif