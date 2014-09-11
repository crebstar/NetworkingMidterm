
#include "OpenGLShader.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "EngineCommon.hpp"

#include "OpenGLShaderError.hpp"

#include <stdio.h>
#include <cassert>

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

OpenGLShader::~OpenGLShader() {

}

OpenGLShader::OpenGLShader( const std::string & shaderFilePath, const std::string & shaderFileName, ShaderType shaderType ) {

	m_shaderFilePath = shaderFilePath;
	m_shaderFileName = shaderFileName;
	m_shaderType = shaderType;

	loadShaderFromFile();

}

void OpenGLShader::loadShaderFromFile() {

	unsigned char * shaderTextBuffer = NULL;
	int bytesRead = loadShaderFileToNewBuffer( m_shaderFilePath, shaderTextBuffer );

	if ( bytesRead > 0 ) {
		// Success Reading File
		switch ( m_shaderType ) {
		case VERTEX_SHADER:

			m_shaderID = glCreateShader( GL_VERTEX_SHADER );
			break;
		case FRAGMENT_SHADER:

			m_shaderID = glCreateShader( GL_FRAGMENT_SHADER );
			break;
		case GEOMETRY_SHADER:

			m_shaderID = glCreateShader( GL_GEOMETRY_SHADER );
			break;
		default:
			assert( true == true );
			break;

		} // end switch
		
		char * shaderText = (char*) shaderTextBuffer;

		glShaderSource( m_shaderID, 1, &shaderText, nullptr );
		glCompileShader( m_shaderID );

		GLint wasSuccessful;
		glGetShaderiv( m_shaderID, GL_COMPILE_STATUS, &wasSuccessful );
		if ( wasSuccessful != GL_TRUE ) {
			
			//ReportShaderError( m_shaderType, m_shaderID, (char*) shaderText, m_shaderFileName, m_shaderFilePath );
			GLint infoLogLength;
			glGetShaderiv( m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
			char * infoLogText = new char[ infoLogLength + 1 ];
			glGetShaderInfoLog( m_shaderID, infoLogLength, NULL, infoLogText );
			
			OpenGLShaderError shaderError( *(this), infoLogLength, infoLogText );
			
		}

	} else {
		// Error Reading File

	}

	if( shaderTextBuffer ) {
		delete shaderTextBuffer;
	}

}

int OpenGLShader::loadShaderFileToNewBuffer( const std::string & shaderFilePath, unsigned char *& shaderTextBuffer ) {

	int numBytesReadFromFile = 0;

	errno_t error;
	FILE * shaderFile;
	error = fopen_s( &shaderFile, shaderFilePath.c_str(), "rb" );

	if ( error != 0 || shaderFile == nullptr ) {
		return numBytesReadFromFile;
	} 

	fseek( shaderFile, 0L, SEEK_END);
	long byteSizeOfShaderFile = ftell( shaderFile );
	fseek(shaderFile, 0L, SEEK_SET);
	numBytesReadFromFile = static_cast<unsigned int>( byteSizeOfShaderFile );

	shaderTextBuffer = new unsigned char[ byteSizeOfShaderFile + 1 ];
	shaderTextBuffer[ byteSizeOfShaderFile ] = '\0';

	size_t resultOfFileRead;
	resultOfFileRead = fread( shaderTextBuffer, sizeof( char ), sizeof( char ) * byteSizeOfShaderFile, shaderFile );

	fclose( shaderFile );

	return numBytesReadFromFile;

}