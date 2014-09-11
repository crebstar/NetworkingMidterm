#include "CBSkinnedTriangleBatch.hpp"

#include "CBMesh.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLShaderProgram.hpp"
#include "OpenGLShaderError.hpp"
#include "OpenGLShaderManager.hpp"

#include "EntityMesh.hpp"
#include "CBSkinnedMesh.hpp"
#include "Light.hpp"

#include "Vector2.hpp"

#include "MatrixStack.hpp"
#include "TextureManager.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CBSkinnedTriangleBatch::~CBSkinnedTriangleBatch() {

}


CBSkinnedTriangleBatch::CBSkinnedTriangleBatch( CBSkinnedMesh* parentMesh, const std::vector<cbengine::Vertex>& verts ) {

	setCBSkinnedTriangleBatchDefaults();
	assert( parentMesh != nullptr );
	m_parentMesh = parentMesh;

	m_verts = verts;
}


void CBSkinnedTriangleBatch::render( float deltaSeconds, const std::vector<Matrix44<float>>& boneTransformationMatrices ) const {

	UNUSED( deltaSeconds );
	m_material->updateUniformValue( m_boneTMUniformLocation, boneTransformationMatrices );
	updateShaderWithLightData();

	if ( m_validForRender ) {
		cbengine::DrawPrimitive drawPrim = cbengine::kTRIANGLES;
		m_material->setUpRenderingState( m_VBOBufferID, drawPrim, m_verts );
	}

}


void CBSkinnedTriangleBatch::createVBOAndMaterial() {

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &m_VBOBufferID );
	sharedRenderer->bindVBOBuffer( m_VBOBufferID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_verts.size() ), &m_verts.front() );

	createMaterial();

	m_validForRender = true;
}


void CBSkinnedTriangleBatch::createMaterial() {

	OpenGLShaderManager& shaderManager = OpenGLShaderManager::getOpenGLShaderManager();

	// Note going to remove this function later and move everything to renderer
	std::string vertexFilePath = "Shaders/SkinnedMeshShader_330.vertex.glsl";
	std::string vertexFileName = "SkinnedMeshShader_330.vertex.glsl";
	std::string fragmentFilePath = "Shaders/SkinnedMeshShader_330.fragment.glsl";
	std::string fragmentFileName = "SkinnedMeshShader_330.fragment.glsl";

	OpenGLShaderProgram* shaderProgram = shaderManager.generateOrGrabExistingShaderProgram( vertexFilePath, vertexFileName, fragmentFilePath, fragmentFileName );

	if ( shaderProgram != nullptr ) {
		m_material = new Material( shaderProgram );
	}

	if ( m_diffuseTexture != nullptr ) {
		m_material->addTextureToMaterial(Material::DiffuseTextureSlot, m_diffuseTexture->getGLTextureID() );
	}

	if ( m_normalMapTexture != nullptr ) {
		m_material->addTextureToMaterial( Material::NormalMapTextureSlot, m_normalMapTexture->getGLTextureID() );
	}

	if ( m_specularLevelTexture != nullptr ) {
		m_material->addTextureToMaterial( Material::SpecularLevelMapTextureSlot, m_specularLevelTexture->getGLTextureID() );
	}

	if ( m_specularColorTexture != nullptr ) {
		m_material->addTextureToMaterial( Material::SpecularColorMapTextureSlot, m_specularColorTexture->getGLTextureID() );
	}

	if ( m_emissiveTexture != nullptr ) {
		m_material->addTextureToMaterial( Material::EmissiveTextureSlot, m_emissiveTexture->getGLTextureID() );
	}

}

void CBSkinnedTriangleBatch::setCBSkinnedTriangleBatchDefaults() {

	m_parentMesh = nullptr;
	m_diffuseTexture = nullptr;
	m_specularColorTexture = nullptr;
	m_specularLevelTexture = nullptr;
	m_emissiveTexture = nullptr;
	m_normalMapTexture = nullptr;
	m_boneTMUniformLocation = -1;
}


void CBSkinnedTriangleBatch::setDiffuseTexture( const std::string& diffuseTextureName ) {

	if ( m_diffuseTexture ) {
		m_diffuseTexture = nullptr; // TODO:: Consider releasing the texture
	}

	cbengine::TextureManager* sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	m_diffuseTexture = sharedTextureManager->generateOrGrabExistingTexture( diffuseTextureName );

}


void CBSkinnedTriangleBatch::setNormalMapTexture( const std::string& normalMapTexture ) {

	if ( m_normalMapTexture ) {
		m_normalMapTexture = nullptr;
	}

	cbengine::TextureManager* sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	m_normalMapTexture = sharedTextureManager->generateOrGrabExistingTexture( normalMapTexture );

}


void CBSkinnedTriangleBatch::setSpecularLevelTexture( const std::string& specularTexture ) {

	if ( m_specularLevelTexture ) {
		m_specularLevelTexture = nullptr;
	}

	cbengine::TextureManager* sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	m_specularLevelTexture = sharedTextureManager->generateOrGrabExistingTexture( specularTexture );
}


void CBSkinnedTriangleBatch::setSpecularColorTexture( const std::string& specularColorTexture ) {

	if ( m_specularColorTexture != nullptr ) {
		m_specularColorTexture = nullptr;
	}

	cbengine::TextureManager* sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	m_specularColorTexture = sharedTextureManager->generateOrGrabExistingTexture( specularColorTexture );
}



void CBSkinnedTriangleBatch::setEmissiveTexture( const std::string& emissiveTexture ) {

	if ( m_emissiveTexture != nullptr ) {
		m_emissiveTexture = nullptr;
	}

	cbengine::TextureManager* sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	m_emissiveTexture = sharedTextureManager->generateOrGrabExistingTexture( emissiveTexture );
}


void CBSkinnedTriangleBatch::updateShaderWithLightData() const {


	const EntityMesh* entityMesh = m_parentMesh->getEntityMesh();
	const std::vector<Light>& m_meshLights = entityMesh->m_lightsEffectingMesh;
	m_material->updateLightValues( m_meshLights );
}