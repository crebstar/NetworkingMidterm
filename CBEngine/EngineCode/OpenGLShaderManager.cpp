#include "OpenGLShaderManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

OpenGLShaderManager::~OpenGLShaderManager() {

}


OpenGLShaderProgram* OpenGLShaderManager::generateOrGrabExistingShaderProgram( const std::string& vertexFilePath,
	const std::string& vertexFileName,
	const std::string& fragmentFilePath,
	const std::string& fragmentFileName ) 
{

	OpenGLShaderProgram* shaderProgramToReturn = nullptr;

	std::string shaderProgramKey = vertexFileName;
	shaderProgramKey += fragmentFileName;

	std::map<std::string,OpenGLShaderProgram*>::iterator it;
	it = m_openGLShaders.find( shaderProgramKey );

	if ( it != m_openGLShaders.end() ) {
		shaderProgramToReturn = it->second;
		return shaderProgramToReturn;
	}

	ShaderType shaderType = VERTEX_SHADER;
	OpenGLShader* vertexShader = new OpenGLShader( vertexFilePath, vertexFileName, shaderType );
	shaderType = FRAGMENT_SHADER;
	OpenGLShader* fragmentshader = new OpenGLShader( fragmentFilePath, fragmentFileName, shaderType );

	OpenGLShaderProgram* shaderProgram = new OpenGLShaderProgram( *( vertexShader ), *( fragmentshader ) );
	shaderProgramKey.clear();
	shaderProgramKey = vertexFileName;
	shaderProgramKey += fragmentFileName;

	m_openGLShaders.insert( std::pair<std::string,OpenGLShaderProgram*>( shaderProgramKey, shaderProgram ) );

	return shaderProgram;
}