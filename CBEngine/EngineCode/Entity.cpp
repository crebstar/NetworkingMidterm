
#include "Entity.hpp"


#include <stdlib.h>
#include "../EngineCode/MathUtil.hpp"
#include "Matrix44.hpp"
#include "MatrixStack.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLShaderProgram.hpp"
#include "OpenGLShaderError.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Entity3D::~Entity3D() {

	if ( m_material ) {
		delete m_material;
	}

}

Entity3D::Entity3D() {

	m_VBOBufferID = 0;
	m_diffuseTexture = nullptr;
	m_material = nullptr;

}


Entity3D * Entity3D::createEntityFromVertexVector( const std::vector< cbengine::Vertex > & vertVec ) {
	Entity3D * entityToCreate = new Entity3D;
	entityToCreate->m_verts = vertVec;

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &entityToCreate->m_VBOBufferID );
	sharedRenderer->bindVBOBuffer( entityToCreate->m_VBOBufferID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * entityToCreate->m_verts.size() ), &entityToCreate->m_verts.front() );

	return entityToCreate;

}


Entity3D* Entity3D::createEntityFromVertexVector( const std::vector<cbengine::Vertex>& vertVec, const std::string& diffuseTextureName ) {
	Entity3D * entityToCreate = new Entity3D;
	entityToCreate->m_verts = vertVec;

	if ( diffuseTextureName != "None" ) {
		cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
		cbengine::Texture * entityDiffuseTexture = sharedTextureManager->generateOrGrabExistingTexture( diffuseTextureName );
		assert( entityDiffuseTexture != nullptr );
		entityToCreate->m_diffuseTexture = entityDiffuseTexture;
	}
	
	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &entityToCreate->m_VBOBufferID );
	sharedRenderer->bindVBOBuffer( entityToCreate->m_VBOBufferID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * entityToCreate->m_verts.size() ), &entityToCreate->m_verts.front() );

	return entityToCreate;
}


void Entity3D::createMaterial() {
	// Note going to remove this function later and move everything to renderer
	std::string shaderFilePath = "Shaders/SimpleShader_110.vertex.glsl";
	std::string shaderFileName = "SimpleShader_110.vertex.glsl";
	ShaderType shaderType = VERTEX_SHADER;
	OpenGLShader* simpleVertexShader = new OpenGLShader( shaderFilePath, shaderFileName, shaderType );

	shaderFilePath = "Shaders/SimpleShader_110.fragment.glsl";
	shaderFileName = "SimpleShader_110.fragment.glsl";
	shaderType = FRAGMENT_SHADER;

	OpenGLShader* simpleFragmentShader = new OpenGLShader( shaderFilePath, shaderFileName, shaderType );

	OpenGLShaderProgram* simpleOpenGLShaderProgram = new OpenGLShaderProgram( *( simpleVertexShader ), *( simpleFragmentShader ) );

	m_material = new Material( simpleOpenGLShaderProgram );
	if ( m_diffuseTexture != nullptr ) {
		m_material->addTextureToMaterial(Material::DiffuseTextureSlot, m_diffuseTexture->getGLTextureID() );
	}

}

Entity3D * Entity3D::createEntityWithFullScreenQuad( float screenWidth, float screenHeight ) {

	Entity3D * entityToCreate = new Entity3D;

	cbengine::Vertex quadVerts;
	quadVerts.vertexColor.x = 1.0f;
	quadVerts.vertexColor.y = 1.0f;
	quadVerts.vertexColor.z = 1.0f;
	quadVerts.vertexColor.w = 1.0f;

	// South West
	quadVerts.vertexTextureCoords.x = 0.0f;
	quadVerts.vertexTextureCoords.y = 0.0f;
	quadVerts.vertexPosition.x = 0.0f;
	quadVerts.vertexPosition.y = 0.0f;
	quadVerts.vertexPosition.z = 0.0f;
	quadVerts.vertexNormal.x = -1.0f;
	quadVerts.vertexNormal.y = 0.0f;
	quadVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( quadVerts );

	// SouthEast
	quadVerts.vertexTextureCoords.x = 1.0f;
	quadVerts.vertexTextureCoords.y = 0.0f;
	quadVerts.vertexPosition.x = screenWidth;
	quadVerts.vertexPosition.y = 0.0f;
	quadVerts.vertexPosition.z = 0.0f;
	quadVerts.vertexNormal.x = -1.0f;
	quadVerts.vertexNormal.y = 0.0f;
	quadVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( quadVerts );

	// NorthEast
	quadVerts.vertexTextureCoords.x = 1.0f;
	quadVerts.vertexTextureCoords.y = 1.0f;
	quadVerts.vertexPosition.x = screenWidth;
	quadVerts.vertexPosition.y = screenHeight;
	quadVerts.vertexPosition.z = 0.0f;
	quadVerts.vertexNormal.x = -1.0f;
	quadVerts.vertexNormal.y = 0.0f;
	quadVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( quadVerts );

	// NorthWest
	quadVerts.vertexTextureCoords.x = 0.0f;
	quadVerts.vertexTextureCoords.y = 1.0f;
	quadVerts.vertexPosition.x = 0.0f;
	quadVerts.vertexPosition.y = screenHeight;
	quadVerts.vertexPosition.z = 0.0f;
	quadVerts.vertexNormal.x = -1.0f;
	quadVerts.vertexNormal.y = 0.0f;
	quadVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( quadVerts );


	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &entityToCreate->m_VBOBufferID );
	sharedRenderer->bindVBOBuffer( entityToCreate->m_VBOBufferID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * entityToCreate->m_verts.size() ), &entityToCreate->m_verts.front() );

	return entityToCreate;
}


Entity3D * Entity3D::createEntityWithCubeGeometry( float xlength, float ywidth, float zheight, const std::string & diffuseTextureName ) {

	Entity3D * entityToCreate = new Entity3D;
	cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	cbengine::Texture * entityDiffuseTexture = sharedTextureManager->generateOrGrabExistingTexture( diffuseTextureName );
	assert( entityDiffuseTexture != nullptr );
	entityToCreate->m_diffuseTexture = entityDiffuseTexture;

	// For tangent and bitangent calcs
	const size_t stepAmount = 4;
	size_t currentOffset = 0;

	// Assumes origin is southeast bottom corner
	cbengine::Vertex cubeVerts;
	cubeVerts.vertexColor.x = 1.0f;
	cubeVerts.vertexColor.y = 1.0f;
	cubeVerts.vertexColor.z = 1.0f;
	cubeVerts.vertexColor.w = 1.0f;

	// South Face
	// Southwest
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = -1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	// SouthEast
	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = -1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	// NorthEast
	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = -1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	// NorthWest
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = -1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cbengine::Vertex * currentVertex = &entityToCreate->m_verts[currentOffset];
	cbengine::Vertex * adjBehind = &entityToCreate->m_verts[currentOffset + (stepAmount-1)];
	cbengine::Vertex * adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);
	currentOffset++;

	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset+1 ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset - (stepAmount-1) ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);
	

	// East Face
	// Southwest
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = -1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	// SouthEast
	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = -1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	// NorthEast
	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = -1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	// NorthWest
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = -1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	currentOffset++;
	currentVertex = &entityToCreate->m_verts[currentOffset];
	adjBehind = &entityToCreate->m_verts[ currentOffset + (stepAmount-1) ];
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);
	currentOffset++;

	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset+1 ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset - (stepAmount-1) ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	// North Face
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 1.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	currentOffset++;
	currentVertex = &entityToCreate->m_verts[currentOffset];
	adjBehind = &entityToCreate->m_verts[ currentOffset + (stepAmount-1) ];
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);
	currentOffset++;

	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset+1 ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset - (stepAmount-1) ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	// West Face
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 1.0f;
	cubeVerts.vertexNormal.z = 0.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	currentOffset++;
	currentVertex = &entityToCreate->m_verts[currentOffset];
	adjBehind = &entityToCreate->m_verts[ currentOffset + (stepAmount-1) ];
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);
	currentOffset++;

	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset+1 ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset - (stepAmount-1) ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	// Top Face
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = zheight;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = 1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	currentOffset++;
	currentVertex = &entityToCreate->m_verts[currentOffset];
	adjBehind = &entityToCreate->m_verts[ currentOffset + (stepAmount-1) ];
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);
	currentOffset++;

	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset+1 ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset - (stepAmount-1) ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	// Bottom Face
	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = -1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 1.0f;
	cubeVerts.vertexPosition.x = xlength;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = -1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 1.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = 0.0f;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = -1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	cubeVerts.vertexTextureCoords.x = 0.0f;
	cubeVerts.vertexTextureCoords.y = 0.0f;
	cubeVerts.vertexPosition.x = 0.0f;
	cubeVerts.vertexPosition.y = ywidth;
	cubeVerts.vertexPosition.z = 0.0f;
	cubeVerts.vertexNormal.x = 0.0f;
	cubeVerts.vertexNormal.y = 0.0f;
	cubeVerts.vertexNormal.z = -1.0f;
	entityToCreate->m_verts.push_back( cubeVerts );

	currentOffset++;
	currentVertex = &entityToCreate->m_verts[currentOffset];
	adjBehind = &entityToCreate->m_verts[ currentOffset + (stepAmount-1) ];
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);
	currentOffset++;

	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[currentOffset+1];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset+1 ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	currentOffset++;
	adjBehind = currentVertex;
	currentVertex = adjFront;
	adjFront = &entityToCreate->m_verts[ currentOffset - (stepAmount-1) ];
	cbengine::computeSurfaceTangentsAtVertex( (currentVertex->vertexTangent), 
		(currentVertex->vertexBitangent), 
		(currentVertex->vertexNormal), 
		(currentVertex->vertexPosition), 
		(currentVertex->vertexTextureCoords), 
		(adjBehind->vertexPosition), (adjBehind->vertexTextureCoords), 
		(adjFront->vertexPosition), (adjFront->vertexTextureCoords)
		);

	computeTangentAndBitangentVectorsForVerts( entityToCreate );

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &entityToCreate->m_VBOBufferID );
	sharedRenderer->bindVBOBuffer( entityToCreate->m_VBOBufferID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * entityToCreate->m_verts.size() ), &entityToCreate->m_verts.front() );

	return entityToCreate;

} // end factory function


void Entity3D::computeTangentAndBitangentVectorsForVerts( Entity3D * entity) {
	UNUSED( entity );

}


void Entity3D::update( float deltaSeconds ) {
	UNUSED(deltaSeconds);

}

void Entity3D::render( float deltaSeconds ) const {
	UNUSED(deltaSeconds);
	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();
	matrixStack->applyTranslationAndPushToStack( m_position );
	cbengine::DrawPrimitive drawPrim = cbengine::kTRIANGLES;
	m_material->setUpRenderingState( m_VBOBufferID, drawPrim, m_verts );
	matrixStack->popFromTopOfStack();
}


Entity3D * Entity3D::createEntityWithSphereGeometry( float radius, float slices, float stacks, const std::string & diffuseTextureName ) {
	// Paul : My modified version of function to obtain verts for 3d sphere ( Source Below )
	// http://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c

	Entity3D * sphereEntity = new Entity3D;

	float const R = 1.0f / (float)( slices-1.0f );
	float const S = 1.0f / (float)( stacks-1.0f );
	int r, s;
	cbengine::Vertex currentVertex;
	currentVertex.vertexColor.x = 1.0f;
	currentVertex.vertexColor.y = 1.0f;
	currentVertex.vertexColor.z = 1.0f;
	currentVertex.vertexColor.w = 1.0f;

	for(r = 0; r < slices; r++) {
		for(s = 0; s < stacks; s++) {
			const float y = sin( -(cbengine::PI*0.50f) + cbengine::PI * r * R );
			const float x = cos( 2.0f * cbengine::PI * s * S) * sin( cbengine::PI * r * R );
			const float z = sin( 2.0f * cbengine::PI * s * S) * sin( cbengine::PI * r * R );

			currentVertex.vertexTextureCoords.x = s * S;
			currentVertex.vertexTextureCoords.y = r * R;
			
			currentVertex.vertexPosition.x = x * radius;
			currentVertex.vertexPosition.y = y * radius;
			currentVertex.vertexPosition.z = z * radius;

			// No normals for now
			//*n++ = x;
			//*n++ = y;
			//*n++ = z;

			sphereEntity->m_verts.push_back( currentVertex );

		} // end inner for
	} // end outer for 

	cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();

	cbengine::Texture * entityDiffuseTexture = sharedTextureManager->generateOrGrabExistingTexture( diffuseTextureName );
	assert( entityDiffuseTexture != nullptr );
	sphereEntity->m_diffuseTexture = entityDiffuseTexture;

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &sphereEntity->m_VBOBufferID );
	sharedRenderer->bindVBOBuffer( sphereEntity->m_VBOBufferID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * sphereEntity->m_verts.size() ), &sphereEntity->m_verts.front() );

	return sphereEntity;
}



