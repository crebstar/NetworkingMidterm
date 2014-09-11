
#ifndef included_OpenGLShader
#define included_OpenGLShader
#pragma once

#include <string>


typedef enum {

	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER,

} ShaderType;

class OpenGLShader {

public:

	explicit OpenGLShader( const std::string & shaderFilePath, const std::string & shaderFileName, ShaderType shaderType );
	~OpenGLShader();


	std::string			m_shaderFileName;
	std::string			m_shaderFilePath;
	unsigned int		m_shaderID;
	ShaderType			m_shaderType;

protected:


private:

	void loadShaderFromFile();
	int loadShaderFileToNewBuffer( const std::string & shaderFilePath, unsigned char *& shaderTextBuffer );

};

#endif