
#ifndef included_OpenGLShaderError
#define included_OpenGLShaderError
#pragma once

#include <string>

#include "EngineMacros.hpp"

class OpenGLShader;
class OpenGLShaderProgram;

const std::string SHADER_TYPE_VERTEX_STRING				= "Vertex Shader";
const std::string SHADER_TYPE_FRAGMENT_STRING			= "Fragment Shader";
const std::string SHADER_TYPE_GEOMETRY_STRING			= "Geometry Shader";
const std::string SHADER_ERROR_START_MSG				= "SHADER ERROR: An error has occurred with ";
const std::string SHADER_DIALOG_BOX_TITLE				= "Shader Compile Error";

const size_t LINE_NUMBER_PARSE_SANITY_CHECK				= 1000;

class OpenGLShaderError {

public:

	~OpenGLShaderError();
	explicit OpenGLShaderError( const OpenGLShader & shaderWithError, int lengthOfLogText, char * shaderErrorText );
	explicit OpenGLShaderError( const OpenGLShaderProgram & shaderProgramWithError, 
		const OpenGLShader & vertexShader, 
		const OpenGLShader & fragmentShader, 
		int lengthOfLogText, 
		char * shaderErrorText );

protected:


private:
	PREVENT_COPY_AND_ASSIGN( OpenGLShaderError );

	void reportShaderCompileError( const OpenGLShader & shaderWithError, int lengthOfLogText, char * shaderErrorText );
	void reportShaderProgramLinkError( const OpenGLShaderProgram & shaderProgramWithError, 
		const OpenGLShader & vertexShader, 
		const OpenGLShader & fragmentShader, 
		int lengthOfLogText, 
		char * shaderErrorText );
	size_t extractLineNumberFromErrorString( size_t index, char * errorMessage, std::string & lineNumber );

};

#endif