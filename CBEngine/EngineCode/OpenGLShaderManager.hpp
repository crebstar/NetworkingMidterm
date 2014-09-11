#ifndef included_OpenGLShaderManager
#define included_OpenGLShaderManager
#pragma once

#include <map>
#include <string>
#include "OpenGLShaderProgram.hpp"
#include "OpenGLShaderError.hpp"
#include "OpenGLShader.hpp"

#include "EngineCommon.hpp"

class OpenGLShaderManager {
public:

	static OpenGLShaderManager& getOpenGLShaderManager() {
		static OpenGLShaderManager shaderManager;
		return shaderManager;
	}

	~OpenGLShaderManager();

	OpenGLShaderProgram* generateOrGrabExistingShaderProgram( const std::string& vertexFilePath,
		const std::string& vertexFileName,
		const std::string& fragmentFilePath,
		const std::string& fragmentFileName );

protected:
	OpenGLShaderManager() {}

	
private:
	PREVENT_COPY_AND_ASSIGN( OpenGLShaderManager );

	std::map<std::string,OpenGLShaderProgram*>		m_openGLShaders;
};

#endif