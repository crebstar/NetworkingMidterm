#include "Material.hpp"

#include <cassert>

#include "Fog.hpp"
#include "MatrixStack.hpp"
#include "OpenGLRenderer.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Material::~Material() {

	if ( m_fog != nullptr ) {
		delete m_fog;
	}
}


Material::Material( OpenGLShaderProgram * shaderProgram ) {
	assert( shaderProgram != nullptr );
	m_shaderProgram = shaderProgram;

	// Determine Locations
	// Attributes
	m_diffuseTextureCoordLocation = glGetAttribLocation( m_shaderProgram->m_programID, DIFFUSE_ATT_NAME.c_str() );
	m_vertexLocation = glGetAttribLocation( m_shaderProgram->m_programID, VERTEX_ATT_NAME.c_str() );
	m_colorLocation = glGetAttribLocation( m_shaderProgram->m_programID, COLOR_ATT_NAME.c_str() );
	m_normalCoordLocation = glGetAttribLocation( m_shaderProgram->m_programID, NORMAL_ATT_NAME.c_str() );
	m_tangentLocation = glGetAttribLocation( m_shaderProgram->m_programID, TANGENT_ATT_NAME.c_str() );
	m_bitangentLocation = glGetAttribLocation( m_shaderProgram->m_programID, BITANGENT_ATT_NAME.c_str() );

	// Bone Indexes and Weights
	m_boneIndexesLocation = glGetAttribLocation( m_shaderProgram->m_programID, BONE_INDEXES_ATT_NAME.c_str() );
	m_boneWeightsLocation = glGetAttribLocation( m_shaderProgram->m_programID, BONE_WEIGHTS_ATT_NAME.c_str() );

	// Uniforms
	m_mvpMatrixLocation = glGetUniformLocation( m_shaderProgram->m_programID, MVP_UNIFORM_NAME.c_str() );
	m_cameraSpaceToWorldSpaceTMLocation = glGetUniformLocation( m_shaderProgram->m_programID, CAMERA_SPACE_TO_WORLD_SPACE_TM_NAME.c_str() );
	m_worldSpaceToCameraSpaceTMLocation = glGetUniformLocation( m_shaderProgram->m_programID, WORLD_SPACE_TO_CAMERA_SPACE_TM_NAME.c_str() );
	m_modelTransformLocation = glGetUniformLocation( m_shaderProgram->m_programID, MODEL_TRANSFORM_NAME.c_str() );
	m_projectionMatrixLocation = glGetUniformLocation( m_shaderProgram->m_programID, PROJECTION_MATRIX_NAME.c_str() );
	m_diffuseTextureLocation = glGetUniformLocation( m_shaderProgram->m_programID, DIFFUSE_UNIFORM_NAME.c_str() );
	m_normalTextureLocation = glGetUniformLocation( m_shaderProgram->m_programID, NORMAL_UNIFORM_NAME.c_str() );
	m_specularLevelTextureLocation = glGetUniformLocation( m_shaderProgram->m_programID, SPECULAR_LEVEL_UNIFORM_NAME.c_str() );
	m_specularColorTextureLocation = glGetUniformLocation( m_shaderProgram->m_programID, SPECULAR_COLOR_UNIFORM_NAME.c_str() );
	m_emissiveTextureLocation = glGetUniformLocation( m_shaderProgram->m_programID, EMISSIVE_UNIFORM_NAME.c_str() );
	m_depthTextureLocation = glGetUniformLocation( m_shaderProgram->m_programID, DEPTH_UNIFORM_NAME.c_str() );
	m_useTexturesLocation = glGetUniformLocation( m_shaderProgram->m_programID, USE_TEXTURE_UNIFORM_NAME.c_str() );
	m_cameraPositionLocation = glGetUniformLocation( m_shaderProgram->m_programID, CAMERA_POSITION_NAME.c_str() );

	determineLightUniformLocations();

	m_fog = nullptr;
}


void Material::determineLightUniformLocations() {
	
	// Light Uniforms
	m_lightArrayUniformLocation = glGetUniformLocation( m_shaderProgram->m_programID, DYNAMIC_LIGHT_ARRAY_UNIFORM_NAME.c_str() );
	m_numLightsUniformLocation = glGetUniformLocation( m_shaderProgram->m_programID, NUM_DYNAMIC_LIGHTS_IN_USE.c_str() );

	// Struct Values For Light Struct
	for ( size_t i = 0; i < TOTAL_DYNAMIC_LIGHTS; ++i ) {
		std::string stringToConcat;
		std::string currentLoopNum = std::to_string( static_cast<_ULONGLONG >( i ) );
		std::string uniformBase = DYNAMIC_LIGHT_ARRAY_UNIFORM_NAME + "[" + currentLoopNum + "].";
	
		// Position
		std::string positionLocation;
		positionLocation += uniformBase;
		positionLocation += "position";
		int positionLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, positionLocation.c_str() );
		m_lightPositionLocations.push_back( positionLocationValue );
		// Direction
		std::string directionLocation;
		directionLocation += uniformBase;
		directionLocation += "direction";
		int lightDirectionLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, directionLocation.c_str() );
		m_lightDirectionLocations.push_back( lightDirectionLocationValue );
		// Color and Brightness
		std::string colorAndBrightnessLocation;
		colorAndBrightnessLocation += uniformBase;
		colorAndBrightnessLocation += "colorAndBrightness";
		int colorAndBrightnessLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, colorAndBrightnessLocation.c_str() );
		m_lightColorAndBrightnessLocations.push_back( colorAndBrightnessLocationValue );
		// inner Radius
		std::string innerRadiusLocation;
		innerRadiusLocation += uniformBase;
		innerRadiusLocation += "innerRadius";
		int innerRadiusLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, innerRadiusLocation.c_str() );
		m_lightInnerRadiusLocations.push_back( innerRadiusLocationValue );
		// outer Radius
		std::string outerRadiusLocation;
		outerRadiusLocation += uniformBase;
		outerRadiusLocation += "outerRadius";
		int outerRadiusLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, outerRadiusLocation.c_str() );
		m_lightOuterRadiusLocations.push_back( outerRadiusLocationValue );
		// inner Apperture Dot
		std::string innerAppertureDotLocation;
		innerAppertureDotLocation += uniformBase;
		innerAppertureDotLocation += "innerApertureDot";
		int innerAppertureDotLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, innerAppertureDotLocation.c_str() );
		m_lightInnerApertureDotLocations.push_back( innerAppertureDotLocationValue );
		// outer Apperture Dot
		std::string outerAppertureDotLocation;
		outerAppertureDotLocation += uniformBase;
		outerAppertureDotLocation += "outerApertureDot";
		int outerAppertureDotLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, outerAppertureDotLocation.c_str() );
		m_lightOuterApertureDotLocations.push_back( outerAppertureDotLocationValue );
		// fraction Ambience
		std::string fractionAmbienceLocation;
		fractionAmbienceLocation += uniformBase;
		fractionAmbienceLocation += "fractionAmbient";
		int fractionAmbienceLocationValue = glGetUniformLocation( m_shaderProgram->m_programID, fractionAmbienceLocation.c_str() );
		m_lightFractionAmbienceLocations.push_back( fractionAmbienceLocationValue );
		// ignores position
		std::string ignoresPosition;
		ignoresPosition += uniformBase;
		ignoresPosition += "ignoresPosition";
		int ignoresPositionValue = glGetUniformLocation( m_shaderProgram->m_programID, ignoresPosition.c_str() );
		m_lightIgnoresPositionLocations.push_back( ignoresPositionValue );
		// is local ambient
		std::string isLocalAmbient;
		isLocalAmbient += uniformBase;
		isLocalAmbient += "isLocalAmbient";
		int isLocalAmbientValue = glGetUniformLocation( m_shaderProgram->m_programID, isLocalAmbient.c_str() );
		m_lightIsLocalAmbientLocations.push_back( isLocalAmbientValue );

	}

}


void Material::updateLightValues( const std::vector< Light > & lights ) {

	glUseProgram( m_shaderProgram->m_programID );

	for ( size_t i = 0; i < lights.size(); ++i ) {

		const Light & light = lights[i];
		glUniform3f( m_lightPositionLocations[i], light.m_position.x, light.m_position.y, light.m_position.z );
		glUniform3f( m_lightDirectionLocations[i], light.m_direction.x, light.m_direction.y, light.m_direction.z );
		//glUniform4f( m_lightColorAndBrightnessLocations[i], 
		//	light.m_colorAndBrightness.x, 
		//	light.m_colorAndBrightness.y, 
		//	light.m_colorAndBrightness.z, 
		//	light.m_colorAndBrightness.w );
		float colorArray[4];
		colorArray[0] = light.m_colorAndBrightness.x;
		colorArray[1] = light.m_colorAndBrightness.y;
		colorArray[2] = light.m_colorAndBrightness.z;
		colorArray[3] = light.m_colorAndBrightness.w;

		glUniform4fv( m_lightColorAndBrightnessLocations[i], 1, &colorArray[0] );
		glUniform1f( m_lightInnerRadiusLocations[i], light.m_innerRadius );
		glUniform1f( m_lightOuterRadiusLocations[i], light.m_outerRadius );
		glUniform1f( m_lightInnerApertureDotLocations[i], light.m_innerApertureDot );
		glUniform1f( m_lightOuterApertureDotLocations[i], light.m_outerApertureDot );
		glUniform1f( m_lightFractionAmbienceLocations[i], light.m_fractionAmbient );
		glUniform1f( m_lightIgnoresPositionLocations[i], light.m_ignoresPosition );
		glUniform1f( m_lightIsLocalAmbientLocations[i], light.m_isLocalAmbient );
	}

	// Update the number of lights
	glUniform1i( m_numLightsUniformLocation, lights.size() );

}

void Material::setUpRenderingState( unsigned int vboID, cbengine::DrawPrimitive drawPrim, const std::vector< cbengine::Vertex > & verts ) {

	glUseProgram( m_shaderProgram->m_programID );

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	// Send the current mvp matrix to the shader
	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();
	const Matrix44<float> & topMatrix = matrixStack->getMatrixFromTopOfStack();
	glUniformMatrix4fv( m_mvpMatrixLocation, 1, false, topMatrix.matrixData );

	const Matrix44<float>& cameraToWorldSpaceTM = matrixStack->getCameraToWorldSpaceTransformMatrix();
	glUniformMatrix4fv( m_cameraSpaceToWorldSpaceTMLocation, 1, false, cameraToWorldSpaceTM.matrixData );

	const Matrix44<float>& modelTransformMatrix = matrixStack->getCurrentModelTransformMatrix();
	glUniformMatrix4fv( m_modelTransformLocation, 1, false, modelTransformMatrix.matrixData );

	const Matrix44<float>& projectionMatrix = matrixStack->getCurrentProjectionMatrix();
	glUniformMatrix4fv( m_projectionMatrixLocation, 1, false, projectionMatrix.matrixData );

	const cbengine::Vector3<float>& currentCameraPosition = matrixStack->getCurrentCameraPosition();
	glUniform3f( m_cameraPositionLocation, currentCameraPosition.x, currentCameraPosition.y, currentCameraPosition.z );

	sharedRenderer->bindVBOBuffer( vboID );

	bool textureBound = bindAndActivateTextures();
	if ( textureBound ) {
		glUniform1i( m_useTexturesLocation, 1 );
	} else {
		glUniform1i( m_useTexturesLocation, 0 );
	}

	glEnableVertexAttribArray( m_vertexLocation );
	glVertexAttribPointer( m_vertexLocation, 3, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexPosition ) ); 

	glEnableVertexAttribArray( m_colorLocation );
	glVertexAttribPointer( m_colorLocation, 4, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexColor ) );

	glEnableVertexAttribArray( m_diffuseTextureCoordLocation );
	glVertexAttribPointer( m_diffuseTextureCoordLocation, 2, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexTextureCoords ) );

	glEnableVertexAttribArray( m_normalCoordLocation );
	glVertexAttribPointer( m_normalCoordLocation, 3, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexNormal ) );

	glEnableVertexAttribArray( m_tangentLocation );
	glVertexAttribPointer( m_tangentLocation, 3, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexTangent ) );

	glEnableVertexAttribArray( m_bitangentLocation );
	glVertexAttribPointer( m_bitangentLocation, 3, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexBitangent ) );

	glEnableVertexAttribArray( m_boneIndexesLocation ); // Apparently GL_INT causes issues
	glVertexAttribPointer( m_boneIndexesLocation, 4, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexBoneIndexes ) );

	glEnableVertexAttribArray( m_boneWeightsLocation );
	glVertexAttribPointer( m_boneWeightsLocation, 4, GL_FLOAT, false, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexBoneWeights ) );
	
	// TODO :: Make toggle variables for these
	sharedRenderer->disableLighting();
	//sharedRenderer->enableDepthBuffering(); 
	sharedRenderer->enableCullFace();
	sharedRenderer->cullFrontFaceCounterClockwise();
	sharedRenderer->cullFaceBack();

	sendCustomUniformData();
	sendFogDataToShader();

	sharedRenderer->drawArrays( drawPrim, 0, verts.size() );

	glDisableVertexAttribArray( m_vertexLocation );
	glDisableVertexAttribArray( m_colorLocation );
	glDisableVertexAttribArray( m_diffuseTextureCoordLocation );
	glDisableVertexAttribArray( m_normalCoordLocation );
	glDisableVertexAttribArray( m_tangentLocation );
	glDisableVertexAttribArray( m_bitangentLocation );
	glDisableVertexAttribArray( m_boneIndexesLocation );
	glDisableVertexAttribArray( m_boneWeightsLocation );

	glActiveTexture( GL_TEXTURE5 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE4 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE3 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE2 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE1 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );

	sharedRenderer->disable2DTextures();
}


void Material::sendCustomUniformData() {

	std::map< int, Uniform >::iterator itUnif;
	for ( itUnif = m_uniformVariablesMap.begin(); itUnif != m_uniformVariablesMap.end(); ++itUnif ) {

		int shaderLocation = itUnif->first;
		const Uniform & uniform = itUnif->second;
		switch ( uniform.m_uniformType ) {
		case TYPE_INT:
			glUniform1i( shaderLocation, uniform.m_uniformInt );
			break;
		case TYPE_FLOAT:
			glUniform1f( shaderLocation, uniform.m_uniformFloat );
		break;
		case TYPE_VECTOR3:
			
			glUniform3f( shaderLocation, uniform.m_uniformVector.x, uniform.m_uniformVector.y, uniform.m_uniformVector.z );
			
			break;
		case TYPE_MATRIX44:
			glUniformMatrix4fv( shaderLocation, 1, false, uniform.m_uniformMatrix44.matrixData );
			break;
		case TYPE_MATRIX44_ARRAY:

			glUniformMatrix4fv( shaderLocation, NUM_BONE_MATRIX_PASSED_TO_SHADER, false, uniform.m_uniformMatrix44Vector.front().matrixData );
			break;
		default:
			// Oh noes
			break;
		}
	} // end for

}

bool Material::bindAndActivateTextures() {

	bool textureBound = false;
	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	// Diffuse
	std::map< Texture_Slot, cbengine::Texture * >::iterator itDiff;
	itDiff = m_materialTextures.find( DiffuseTextureSlot );
	if ( itDiff != m_materialTextures.end() ) {
		glUniform1i( m_diffuseTextureLocation, 0 );
		glActiveTexture( GL_TEXTURE0 );
		sharedRenderer->bind2DTexture( itDiff->second->getGLTextureID() );
		textureBound = true;
	}

	// Normal
	std::map< Texture_Slot, cbengine::Texture * >::iterator itNormal;
	itNormal = m_materialTextures.find( NormalMapTextureSlot );
	if ( itNormal != m_materialTextures.end() ) {
		glUniform1i( m_normalTextureLocation, 1 );
		glActiveTexture( GL_TEXTURE1 );
		unsigned int texId = itNormal->second->getGLTextureID();
		sharedRenderer->bind2DTexture( texId );
		textureBound = true;
	}

	// Specular Level
	std::map< Texture_Slot, cbengine::Texture * >::iterator itSpec;
	itSpec = m_materialTextures.find( SpecularLevelMapTextureSlot );
	if ( itSpec != m_materialTextures.end() ) {
		glUniform1i( m_specularLevelTextureLocation, 2 );
		glActiveTexture( GL_TEXTURE2 );
		unsigned int texID = itSpec->second->getGLTextureID();
		sharedRenderer->bind2DTexture( texID );
		textureBound = true;
	}

	// Specular Color
	std::map<Texture_Slot, cbengine::Texture*>::iterator itSpecColor;
	itSpecColor = m_materialTextures.find( SpecularColorMapTextureSlot );
	if ( itSpecColor != m_materialTextures.end() ) {
		glUniform1i( m_specularColorTextureLocation, 3 );
		glActiveTexture( GL_TEXTURE3 );
		unsigned int texID = itSpecColor->second->getGLTextureID();
		sharedRenderer->bind2DTexture( texID );
		textureBound = true;
	}

	// Emissive
	std::map< Texture_Slot, cbengine::Texture *>::iterator itEmis;
	itEmis = m_materialTextures.find( EmissiveTextureSlot );
	if ( itEmis != m_materialTextures.end() ) {
		glUniform1i( m_emissiveTextureLocation, 4 );
		glActiveTexture( GL_TEXTURE4 );
		unsigned int texID = itEmis->second->getGLTextureID();
		sharedRenderer->bind2DTexture( texID );
		textureBound = true;
	}

	// Depth
	std::map< Texture_Slot, cbengine::Texture *>::iterator itDepth;
	itDepth = m_materialTextures.find( DepthTextureSlot );
	if ( itDepth != m_materialTextures.end() ) {
		glUniform1i( m_depthTextureLocation, 5 );
		glActiveTexture( GL_TEXTURE5 );
		unsigned int texID = itDepth->second->getGLTextureID();
		sharedRenderer->bind2DTexture( texID );
		textureBound = true;

	}

	if ( textureBound ) {
		sharedRenderer->enable2DTextures();
	}
	return textureBound;
}


void Material::addFogEffectToShader( Fog * fogToAdd ) {

	if ( m_fog != nullptr ) {
		delete m_fog;
	}

	assert( fogToAdd != nullptr );
	m_fog = fogToAdd;

	// Find the locations
	m_fog->m_fogColorLocation = glGetUniformLocation( m_shaderProgram->m_programID, FOG_COLOR_UNIFORM_NAME.c_str() );
	m_fog->m_startDistanceLocation = glGetUniformLocation( m_shaderProgram->m_programID, FOG_START_DIST_NAME.c_str() );
	m_fog->m_endDistanceLocation = glGetUniformLocation( m_shaderProgram->m_programID, FOG_END_DIST_NAME.c_str() );

}


void Material::sendFogDataToShader() {

	if ( m_fog == nullptr ) {
		// No fog is in use
		return;
	}

	glUniform1f( m_fog->m_startDistanceLocation, m_fog->m_startDist );
	glUniform1f( m_fog->m_endDistanceLocation, m_fog->m_endDist );
	glUniform4f( m_fog->m_fogColorLocation, m_fog->m_fogColor.x, m_fog->m_fogColor.y, m_fog->m_fogColor.z, m_fog->m_fogColor.w );


}