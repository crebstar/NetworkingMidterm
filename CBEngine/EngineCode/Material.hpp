#ifndef included_Material
#define included_Material
#pragma once

#include <string>
#include <map>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "EngineMacros.hpp"
#include "EngineCommon.hpp"
#include "OpenGLShaderProgram.hpp"
#include "Uniform.hpp"
#include "Texture.hpp"
#include "TextureManager.hpp"
#include "Vertex.hpp"
#include "Light.hpp"

// All shaders working with this material class must conform to these naming conventions
const std::string MVP_UNIFORM_NAME							= "u_mvpMatrix";
const std::string CAMERA_SPACE_TO_WORLD_SPACE_TM_NAME		= "u_cameraSpaceToWorldSpaceTransform";
const std::string WORLD_SPACE_TO_CAMERA_SPACE_TM_NAME		= "u_worldSpaceToCameraSpaceTransform";
const std::string MODEL_TRANSFORM_NAME						= "u_modelSpaceTransform";
const std::string PROJECTION_MATRIX_NAME					= "u_projectionMatrix";
const std::string CAMERA_POSITION_NAME						= "u_cameraPosition";
const std::string VERTEX_ATT_NAME							= "u_vertex";
const std::string COLOR_ATT_NAME							= "u_color";
const std::string DIFFUSE_ATT_NAME							= "u_diffuseTextureCoords";
const std::string DIFFUSE_UNIFORM_NAME						= "u_diffuseTexture";
const std::string NORMAL_UNIFORM_NAME						= "u_normalTexture";
const std::string SPECULAR_LEVEL_UNIFORM_NAME				= "u_specularTexture";
const std::string SPECULAR_COLOR_UNIFORM_NAME				= "u_specularColorTexture";
const std::string EMISSIVE_UNIFORM_NAME						= "u_emissiveTexture";
const std::string DEPTH_UNIFORM_NAME						= "u_depthTexture";
const std::string BUMP_UNIFORM_NAME							= "u_bumpTexture";
const std::string USE_TEXTURE_UNIFORM_NAME					= "u_useTextures";
// TBN
const std::string TANGENT_ATT_NAME							= "a_tangent";
const std::string BITANGENT_ATT_NAME						= "a_bitangent";
const std::string NORMAL_ATT_NAME							= "a_normal";

const std::string BONE_INDEXES_ATT_NAME						= "a_boneIndexes";
const std::string BONE_WEIGHTS_ATT_NAME						= "a_boneWeights";

// Dynamic Lights
const std::string NUM_DYNAMIC_LIGHTS_IN_USE					= "u_numberOfLights";
const std::string DYNAMIC_LIGHT_ARRAY_UNIFORM_NAME			= "u_lightArray";
const size_t      TOTAL_DYNAMIC_LIGHTS						= 16;

class Fog;

class Material {
public:

	~Material();
	explicit Material( OpenGLShaderProgram * shaderProgram );

	// Scoped Enum
	typedef enum {

		DiffuseTextureSlot,
		NormalMapTextureSlot,
		BumpMapTextureSlot,
		SpecularLevelMapTextureSlot,
		SpecularColorMapTextureSlot,
		EmissiveTextureSlot,
		DepthTextureSlot,
	} Texture_Slot;

	// Assumes translations and rotations are applied to the matrix stack by the object who owns the material
	void setUpRenderingState( unsigned int vboID, cbengine::DrawPrimitive drawPrim, const std::vector< cbengine::Vertex >& verts );

	// Assumes ownership transfer
	void addFogEffectToShader( Fog* fogToAdd );

	// Inline Functions
	void addTextureToMaterial( Texture_Slot slotType, const std::string& textureName );
	void addTextureToMaterial( Texture_Slot slotType, unsigned int textureID );
	// Returns negative 1 if uniform could not be found
	int addUniformToMaterial( const std::string& uniformName, const cbengine::Vector3<float>& uniformVec3 );
	int addUniformToMaterial( const std::string& uniformName, const Matrix44<float>& uniformMat44 );
	int addUniformToMaterial( const std::string& uniformName, int uniformInt );
	int addUniformToMaterial( const std::string& uniformName, float uniformFloat );
	int addUniformToMaterial( const std::string& uniformName, const std::vector<Matrix44<float>>& uniformMat44Vector );

	void updateUniformValue( int uniformLocation, const cbengine::Vector3<float>& uniformVec3 );
	void updateUniformValue( int uniformLocation, const Matrix44<float>& uniformMat44  );
	void updateUniformValue( int uniformLocation, const std::vector<Matrix44<float>>& uniformMat44Vector );
	void updateUniformValue( int uniformLocation, int uniformInt );
	void updateUniformValue( int uniformLocation, float uniformFloat );

	void updateLightValues( const std::vector<Light>& lights );

protected:

private:

	bool bindAndActivateTextures();
	void sendCustomUniformData();
	void sendFogDataToShader();
	void determineLightUniformLocations();

	OpenGLShaderProgram *										m_shaderProgram;
	std::map< Texture_Slot, cbengine::Texture * >				m_materialTextures;
	// Holds the custom uniform values
	std::map< int, Uniform >									m_uniformVariablesMap;
	int															m_mvpMatrixLocation;
	int															m_cameraSpaceToWorldSpaceTMLocation;
	int															m_worldSpaceToCameraSpaceTMLocation;
	int															m_modelTransformLocation;
	int															m_projectionMatrixLocation;

	Fog *														m_fog;

	// Texture Uniforms
	int															m_diffuseTextureLocation;
	int															m_normalTextureLocation;
	int															m_specularLevelTextureLocation;
	int															m_specularColorTextureLocation;
	int															m_emissiveTextureLocation;
	int															m_depthTextureLocation;

	// Attribute Locations
	int															m_vertexLocation;
	int															m_colorLocation;
	int															m_normalCoordLocation;
	int															m_diffuseTextureCoordLocation;
	int															m_useTexturesLocation; 
	int															m_cameraPositionLocation;

	int															m_tangentLocation;
	int															m_bitangentLocation;

	int															m_boneIndexesLocation;
	int															m_boneWeightsLocation;

	// Common Uniforms
	int															m_lightArrayUniformLocation;
	int															m_numLightsUniformLocation;

	// Determined at initialization
	std::vector< int >											m_lightPositionLocations;
	std::vector< int >											m_lightDirectionLocations;
	std::vector< int >											m_lightColorAndBrightnessLocations;
	std::vector< int >											m_lightInnerRadiusLocations;
	std::vector< int >											m_lightOuterRadiusLocations;
	std::vector< int >											m_lightInnerApertureDotLocations;
	std::vector< int >											m_lightOuterApertureDotLocations;
	std::vector< int >											m_lightFractionAmbienceLocations;
	std::vector< int >											m_lightIgnoresPositionLocations;
	std::vector< int >											m_lightIsLocalAmbientLocations;

};

// Inline Functions
inline void Material::addTextureToMaterial( Texture_Slot slotType, const std::string & textureName ) {

	cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	std::map< Texture_Slot, cbengine::Texture * >::iterator it;
	it = m_materialTextures.find( slotType );

	if ( it != m_materialTextures.end() ) {
		// A texture already existed... So replace it
		cbengine::Texture * textureToReplace = sharedTextureManager->generateOrGrabExistingTexture( textureName );
		if ( textureToReplace != nullptr ) {
			it->second = textureToReplace;
		}
	} else {
		// Texture did not exist for this slot
		cbengine::Texture * textureToAdd = sharedTextureManager->generateOrGrabExistingTexture( textureName );
		if ( textureToAdd != nullptr ) {
			m_materialTextures.insert( std::pair< Texture_Slot, cbengine::Texture * >(slotType, textureToAdd ) );
		}
	} // end outer if
} 


inline void Material::addTextureToMaterial( Texture_Slot slotType, unsigned int textureID ) {

	cbengine::Texture * texture = new cbengine::Texture( textureID );
	std::map< Texture_Slot, cbengine::Texture * >::iterator it;
	it = m_materialTextures.find( slotType );

	if ( it != m_materialTextures.end() ) {
		// Texture already existed... replace it
		it->second = texture;
	} else {
		// Texture didnt exist
		m_materialTextures.insert( std::pair< Texture_Slot, cbengine::Texture * >( slotType, texture ) );
	}

}


inline int Material::addUniformToMaterial( const std::string& uniformName, const cbengine::Vector3<float>& uniformVec3 ) {

	GLint uniformLocation = glGetUniformLocation( m_shaderProgram->m_programID, uniformName.c_str() );
	if ( uniformLocation != -1 ) {

		Uniform uniformToAdd;
		uniformToAdd.m_uniformType = TYPE_VECTOR3;
		uniformToAdd.m_uniformVector = uniformVec3;
		uniformToAdd.m_uniformLocation = uniformLocation;
		m_uniformVariablesMap.insert( std::pair< int, Uniform >( uniformLocation, uniformToAdd ) );
	}

	return uniformLocation;
}


inline int Material::addUniformToMaterial( const std::string& uniformName, const Matrix44<float>& uniformMat44 ) {

	GLint uniformLocation = glGetUniformLocation( m_shaderProgram->m_programID, uniformName.c_str() );
	if ( uniformLocation != -1 ) {

		Uniform uniformToAdd;
		uniformToAdd.m_uniformType = TYPE_MATRIX44;
		uniformToAdd.m_uniformMatrix44 = uniformMat44;
		uniformToAdd.m_uniformLocation = uniformLocation;
		m_uniformVariablesMap.insert( std::pair< int, Uniform >( uniformLocation, uniformToAdd ) );
	}

	return uniformLocation;
}

inline int Material::addUniformToMaterial( const std::string& uniformName, const std::vector<Matrix44<float>>& uniformMat44Vector ) {
	// TODO:: make function name more descriptive
	assert( uniformMat44Vector.size() == NUM_BONE_MATRIX_PASSED_TO_SHADER );
	GLint uniformLocation = glGetUniformLocation( m_shaderProgram->m_programID, uniformName.c_str() );
	if ( uniformLocation != -1 ) {

		Uniform uniformToAdd;
		uniformToAdd.m_uniformType = TYPE_MATRIX44_ARRAY;
		uniformToAdd.m_uniformMatrix44Vector = uniformMat44Vector;
		uniformToAdd.m_uniformLocation = uniformLocation;
		
		m_uniformVariablesMap.insert( std::pair<int,Uniform>( uniformLocation, uniformToAdd ) );
	}

	return uniformLocation;
}


inline int Material::addUniformToMaterial( const std::string& uniformName, int uniformInt ) {

	GLint uniformLocation = glGetUniformLocation( m_shaderProgram->m_programID, uniformName.c_str() );
	if ( uniformLocation != -1 ) {

		Uniform uniformToAdd;
		uniformToAdd.m_uniformType = TYPE_INT;
		uniformToAdd.m_uniformInt = uniformInt;
		uniformToAdd.m_uniformLocation = uniformLocation;
		m_uniformVariablesMap.insert( std::pair< int, Uniform >( uniformLocation, uniformToAdd ) );
	}

	return uniformLocation;
}


inline int Material::addUniformToMaterial( const std::string& uniformName, float uniformFloat ) {

	GLint uniformLocation = glGetUniformLocation( m_shaderProgram->m_programID, uniformName.c_str() );
	if ( uniformLocation != -1 ) {

		Uniform uniformToAdd;
		uniformToAdd.m_uniformType = TYPE_FLOAT;
		uniformToAdd.m_uniformFloat = uniformFloat;
		uniformToAdd.m_uniformLocation = uniformLocation;
		m_uniformVariablesMap.insert( std::pair< int, Uniform >( uniformLocation, uniformToAdd ) );
	}

	return uniformLocation;
}


inline void Material::updateUniformValue( int uniformLocation, const cbengine::Vector3<float>& uniformVec3 ) {

	std::map< int, Uniform >::iterator it;
	it = m_uniformVariablesMap.find( uniformLocation );
	if ( it != m_uniformVariablesMap.end() ) {

		glUseProgram( m_shaderProgram->m_programID );
		Uniform & uniform = it->second;
		uniform.m_uniformVector = uniformVec3;
	}
}


inline void Material::updateUniformValue( int uniformLocation, const Matrix44<float>& uniformMat44  ) {

	std::map< int, Uniform >::iterator it;
	it = m_uniformVariablesMap.find( uniformLocation );
	if ( it != m_uniformVariablesMap.end() ) {

		glUseProgram( m_shaderProgram->m_programID );
		Uniform & uniform = it->second;
		uniform.m_uniformMatrix44 = uniformMat44;
	}
}


inline void Material::updateUniformValue( int uniformLocation, const std::vector<Matrix44<float>>& uniformMat44Vector ) {

	assert( uniformMat44Vector.size() == NUM_BONE_MATRIX_PASSED_TO_SHADER );
	std::map<int, Uniform>::iterator it;
	it = m_uniformVariablesMap.find( uniformLocation );
	if ( it != m_uniformVariablesMap.end() ) {

		glUseProgram( m_shaderProgram->m_programID );
		Uniform& uniform = it->second;
		
		for ( size_t i = 0; i < NUM_BONE_MATRIX_PASSED_TO_SHADER; ++i ) {

			uniform.m_uniformMatrix44Vector[i] = uniformMat44Vector[i];
		}
	}
}


inline void Material::updateUniformValue( int uniformLocation, int uniformInt ) {

	std::map< int, Uniform >::iterator it;
	it = m_uniformVariablesMap.find( uniformLocation );
	if ( it != m_uniformVariablesMap.end() ) {

		glUseProgram( m_shaderProgram->m_programID );
		Uniform & uniform = it->second;
		uniform.m_uniformInt = uniformInt;
	}
}


inline void Material::updateUniformValue( int uniformLocation, float uniformFloat ) {

	std::map< int, Uniform >::iterator it;
	it = m_uniformVariablesMap.find( uniformLocation );
	if ( it != m_uniformVariablesMap.end() ) {

		glUseProgram( m_shaderProgram->m_programID );
		Uniform & uniform = it->second;
		uniform.m_uniformFloat = uniformFloat;
	}
}


#endif