
#include "OpenGLShaderError.hpp"

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "OpenGLShader.hpp"
#include "OpenGLShaderProgram.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

OpenGLShaderError::~OpenGLShaderError() {

}

OpenGLShaderError::OpenGLShaderError( const OpenGLShader & shaderWithError, int lengthOfLogText, char * shaderErrorText  ) {

	reportShaderCompileError( shaderWithError, lengthOfLogText, shaderErrorText );

}

OpenGLShaderError::OpenGLShaderError( const OpenGLShaderProgram & shaderProgramWithError,
	const OpenGLShader & vertexShader,
	const OpenGLShader & fragmentShader,
	int lengthOfLogText, 
	char * shaderErrorText ) {

	reportShaderProgramLinkError( shaderProgramWithError, vertexShader, fragmentShader, lengthOfLogText, shaderErrorText );

}


void OpenGLShaderError::reportShaderProgramLinkError( const OpenGLShaderProgram & shaderProgramWithError,
	const OpenGLShader & vertexShader, 
	const OpenGLShader & fragmentShader, 
	int lengthOfLogText, 
	char * shaderErrorText ) {

	UNUSED(lengthOfLogText);
	UNUSED(shaderProgramWithError);

	std::string linkerErrorText;
	linkerErrorText += "Linker Error With Shader Program!\n";
	linkerErrorText += "Failed to link following shader programs:\n";
	linkerErrorText += vertexShader.m_shaderFilePath;
	linkerErrorText += "\n";
	linkerErrorText += fragmentShader.m_shaderFilePath;
	linkerErrorText += "\n\n";
	linkerErrorText += shaderErrorText;

	MessageBoxA(  0, (LPCSTR) linkerErrorText.c_str(), "Shader Linker Error", MB_ICONERROR );
	std::exit(0);

}


void OpenGLShaderError::reportShaderCompileError( const OpenGLShader & shaderWithError, int lengthOfLogText, char * shaderErrorText ) {

	std::string shaderErrorMessage;
	shaderErrorMessage += SHADER_ERROR_START_MSG;

	// Determine which shader string to use
	std::string shaderType;
	switch ( shaderWithError.m_shaderType ) {
	case FRAGMENT_SHADER:
		shaderType = SHADER_TYPE_FRAGMENT_STRING;
		break;
	case VERTEX_SHADER:
		shaderType = SHADER_TYPE_VERTEX_STRING;
		break;
	case GEOMETRY_SHADER:
		shaderType = SHADER_TYPE_GEOMETRY_STRING;
		break;
	default:
		shaderType = "Shader Type Unknown";
		break;
	}

	shaderErrorMessage += shaderType;
	shaderErrorMessage += ": '";
	shaderErrorMessage += shaderWithError.m_shaderFileName;
	shaderErrorMessage += "'.\n";

	shaderErrorMessage += "File path to Shader: ";
	shaderErrorMessage += shaderWithError.m_shaderFilePath;
	shaderErrorMessage += "\n\n";

	// Parse Error Type and Line Number
	size_t charParseCount = 0;
	std::string lineNumber = "";
	size_t returningCharPosition;
	while ( charParseCount < LINE_NUMBER_PARSE_SANITY_CHECK ) {
		// Sanity check incase the error message cannot be recognized

		if ( shaderErrorText[charParseCount] == '(' ) {
			 returningCharPosition = extractLineNumberFromErrorString( charParseCount, shaderErrorText, lineNumber );
			break;
		}

		charParseCount++;
	}

	if ( charParseCount >= LINE_NUMBER_PARSE_SANITY_CHECK ) {
		lineNumber = "Line Number Unknown. Log error text format not recognized.";
		MessageBoxA(  0, (LPCSTR) (shaderErrorMessage.c_str()), (LPCSTR) SHADER_DIALOG_BOX_TITLE.c_str(), MB_ICONERROR );
		std::exit(0);
	} 

	std::string errorTypeString;
	charParseCount = returningCharPosition;
	while ( shaderErrorText[charParseCount] != ':' ) {

		errorTypeString += shaderErrorText[charParseCount];
		++charParseCount;
	}

	std::string errorMessageContentString;
	++charParseCount; // Don't include the ':'
	while ( charParseCount < static_cast<size_t>(lengthOfLogText) ) {
		errorMessageContentString += shaderErrorText[charParseCount];
		++charParseCount;
	}

	// Form final parts of the string
	shaderErrorMessage += "Error Type: ";
	shaderErrorMessage += errorTypeString;
	shaderErrorMessage += "\n";
	shaderErrorMessage += "Line Number: ";
	shaderErrorMessage += lineNumber; 
	shaderErrorMessage += "\n\n";
	shaderErrorMessage += "Error Message:\n";
	shaderErrorMessage += errorMessageContentString;

	std::string consoleOutputString;
	consoleOutputString += shaderWithError.m_shaderFilePath;
	consoleOutputString += "(";
	consoleOutputString += lineNumber;
	consoleOutputString += "):";
	consoleOutputString += errorMessageContentString;
	
	OutputDebugStringA( consoleOutputString.c_str() );

	MessageBoxA(  0, (LPCSTR) (shaderErrorMessage.c_str()), (LPCSTR) SHADER_DIALOG_BOX_TITLE.c_str(), MB_ICONERROR );
	std::exit( 0 );

}

size_t OpenGLShaderError::extractLineNumberFromErrorString( size_t index, char * errorMessage, std::string & lineNumber ) {
	// Assumes passed in with start of parenthesis
	// Returns position of string after semicolon
	char endOfLineNumberMarker = ')';

//	size_t currentIndex = ++index;
	while( errorMessage[index] != endOfLineNumberMarker ) {

		lineNumber += errorMessage[index];
		++index;
	}

	while ( errorMessage[index] != ':' ){
		++index;
		if ( index > LINE_NUMBER_PARSE_SANITY_CHECK ) {
			return index;
		}
	}

	return ++index;
}