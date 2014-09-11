
#include "OpenGLShaderProgram.hpp"

#include "EngineCommon.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLShaderError.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

OpenGLShaderProgram::~OpenGLShaderProgram() {

}

OpenGLShaderProgram::OpenGLShaderProgram( const OpenGLShader & vertexShader, const OpenGLShader & fragmentShader ) {

	m_programID = glCreateProgram();
	glAttachShader( m_programID, vertexShader.m_shaderID );
	glAttachShader( m_programID, fragmentShader.m_shaderID );
	glLinkProgram( m_programID );

	GLint wasSuccessful;
	glGetProgramiv( m_programID, GL_LINK_STATUS, &wasSuccessful );

	if ( wasSuccessful != GL_TRUE ) {
		
		GLint infoLogLength;
		glGetProgramiv( m_programID, GL_INFO_LOG_LENGTH, &infoLogLength );
		char * infoLogText = new char[ infoLogLength + 1 ];
		glGetProgramInfoLog( m_programID, infoLogLength, NULL, infoLogText );
		OpenGLShaderError shaderError( *(this), vertexShader, fragmentShader, infoLogLength, infoLogText );
		

	}

	glUseProgram( m_programID );

}

