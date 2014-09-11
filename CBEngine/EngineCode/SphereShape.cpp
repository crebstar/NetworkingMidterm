#include "SphereShape.hpp"

#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>

#include "TextureManager.hpp"
#include "EngineCommon.hpp"
#include "MathUtil.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

SphereShape::~SphereShape() {

}

SphereShape::SphereShape( float radius, float slices, float stacks, const std::string & diffuseTextureName ) {

	assert( radius >= 0 );
	m_radius = radius;
	m_slices = slices;
	m_stacks = stacks;

	cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	m_diffuseTexture = sharedTextureManager->generateOrGrabExistingTexture( diffuseTextureName );
	assert( m_diffuseTexture != nullptr );

	generateSphereVBOData();


}

void SphereShape::update( float deltaSeconds ) {
	UNUSED(deltaSeconds);

}


void SphereShape::render( float deltaSeconds ) const {
	UNUSED(deltaSeconds);
	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();

	sharedRenderer->pushMatrix();

	sharedRenderer->translateF( m_position );

	sharedRenderer->bindVBOBuffer( m_VBOBufferID );

	cbengine::DataType dataType = cbengine::kTYPE_FLOAT;
	sharedRenderer->setVertexPointer( 3, dataType, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexPosition ) );
	sharedRenderer->setColorPointer( 4, dataType, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexColor ) );
	sharedRenderer->setTexCoordPointer( 2, dataType, sizeof( cbengine::Vertex ), (float*) offsetof( cbengine::Vertex, vertexTextureCoords) );

	cbengine::GraphicsClientState graphicsClientState = cbengine::kVERTEX_ARRAY;
	sharedRenderer->enableClientState( graphicsClientState );
	graphicsClientState = cbengine::kCOLOR_ARRAY;
	sharedRenderer->enableClientState( graphicsClientState );
	graphicsClientState = cbengine::kTEXTURE_COORD_ARRAY;
	sharedRenderer->enableClientState( graphicsClientState );

	// Algor draws clockwise...
	// TODO:: Write renderer functions for clockwise backface culling

	sharedRenderer->disableLighting();

	sharedRenderer->enable2DTextures();
	sharedRenderer->enableDepthBuffering();

	glActiveTexture( GL_TEXTURE0 );
	sharedRenderer->bind2DTexture( m_diffuseTexture->getGLTextureID() );
	
	glDrawElements(GL_QUADS, m_sphere_Indices.size() , GL_UNSIGNED_SHORT, &m_sphere_Indices.front() );
	
	graphicsClientState = graphicsClientState = cbengine::kVERTEX_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kCOLOR_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kTEXTURE_COORD_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );

	sharedRenderer->disable2DTextures();

	sharedRenderer->popMatrix();

}


void SphereShape::generateSphereVBOData() {
	// Paul : My modified version of function to obtain vertex data and indices data for 3d sphere ( Source Below )
	// http://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c

	float const R = 1.0f / (float)( m_slices-1.0f );
	float const S = 1.0f / (float)( m_stacks-1.0f );
	int r, s;
	cbengine::Vertex currentVertex;
	currentVertex.vertexColor.x = 1.0f;
	currentVertex.vertexColor.y = 1.0f;
	currentVertex.vertexColor.z = 1.0f;
	currentVertex.vertexColor.w = 1.0f;

	for(r = 0; r < m_slices; r++) {
		for(s = 0; s < m_stacks; s++) {
			const float y = sin( -(cbengine::PI*0.50f) + cbengine::PI * r * R );
			const float x = cos( 2.0f * cbengine::PI * s * S) * sin( cbengine::PI * r * R );
			const float z = sin( 2.0f * cbengine::PI * s * S) * sin( cbengine::PI * r * R );

			currentVertex.vertexTextureCoords.x = s * S;
			currentVertex.vertexTextureCoords.y = r * R;
			
			currentVertex.vertexPosition.x = x * m_radius;
			currentVertex.vertexPosition.y = y * m_radius;
			currentVertex.vertexPosition.z = z * m_radius;

			m_verts.push_back( currentVertex );

		} // end inner for
	} // end outer for 

	// No normals for now
			//*n++ = x;
			//*n++ = y;
			//*n++ = z;
	
	for(r = 0; r < m_slices; r++) {
		for(s = 0; s < m_stacks; s++) {
			m_sphere_Indices.push_back(static_cast< unsigned short >( r * m_stacks + s ));
			m_sphere_Indices.push_back(static_cast< unsigned short >( r * m_stacks + (s+1) ) );
			m_sphere_Indices.push_back(static_cast< unsigned short >( (r+1) * m_stacks + (s+1) ) );
			m_sphere_Indices.push_back(static_cast< unsigned short >( (r+1) * m_stacks + s ) );
		}
	}


	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &m_VBOBufferID );
	sharedRenderer->bindVBOBuffer( m_VBOBufferID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_verts.size() ), &m_verts.front() );


}