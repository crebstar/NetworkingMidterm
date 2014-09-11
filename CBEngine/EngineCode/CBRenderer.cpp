#include "CBRenderer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment( lib, "glu32" )

#include "OpenGLRenderer.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::CBRenderer;

CBRenderer::~CBRenderer() {

}

// All opengl for now...
void CBRenderer::pushMatrix() const {

	glPushMatrix();

}

void CBRenderer::popMatrix() const {

	glPopMatrix();

}


void CBRenderer::enable2DTextures() const {

	glEnable( GL_TEXTURE_2D );

}

void CBRenderer::disable2DTextures() const {

	glDisable( GL_TEXTURE_2D );
	

}

void CBRenderer::bind2DTexture(  unsigned int textureID ) const {

	glBindTexture( GL_TEXTURE_2D, textureID );

}

void CBRenderer::color3F( const cbengine::Vector3<float> & colorValues ) const {

	glColor3f( colorValues.x, colorValues.y, colorValues.z );

}

void CBRenderer::color4F( const cbengine::Vector4<float> & colorValues ) const {

	glColor4f( colorValues.x, colorValues.y, colorValues.z, colorValues.w );

}

void CBRenderer::translateF( const cbengine::Vector3<float> & translatePosition ) const {

	glTranslatef( translatePosition.x, translatePosition.y, translatePosition.z );

}

void CBRenderer::rotateF( float orientationDegrees, float x, float y, float z ) const {

	glRotatef( orientationDegrees, x, y, z );

}


void CBRenderer::vertex3F(  float x, float y, float z ) const {

	glVertex3f( x, y, z );

}

void CBRenderer::beginDraw( cbengine::DrawPrimitive drawPrim ) const {

	unsigned int drawPrimitive = 0;
	switch ( drawPrim ) {
	case kPOINT:
		drawPrimitive = GL_POINT;
		break;
	case kQUADS:
		drawPrimitive = GL_QUADS;
		break;
	case kLINE_LOOP:
		drawPrimitive = GL_LINE_LOOP;
		break;
	default:
		break;
	}

	glBegin( drawPrimitive );
}

void CBRenderer::endDraw() const {
	
	glEnd();
}

void CBRenderer::setLineWidth( float widthOfLine ) const {

	glLineWidth( widthOfLine );
}


void CBRenderer::clearBuffer() {

	if ( m_shouldClearDepthBuffer ) {

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clearing the color buffer
	} else {
		glClear( GL_COLOR_BUFFER_BIT ); // Clearing the color buffer
	}
}


void CBRenderer::enableClientState( cbengine::GraphicsClientState clientState ) const {

	unsigned int stateToEnable = 0;

	switch ( clientState ) {
	case kVERTEX_ARRAY:

		stateToEnable = GL_VERTEX_ARRAY;
		break;
	case kCOLOR_ARRAY:

		stateToEnable = GL_COLOR_ARRAY;
		break;
	case kTEXTURE_COORD_ARRAY:

		stateToEnable = GL_TEXTURE_COORD_ARRAY;
		break;
	default:

		break;
	} // end switch

	glEnableClientState( stateToEnable );
}

void CBRenderer::disableClientState( cbengine::GraphicsClientState clientState ) const {

	unsigned int stateToDisable = 0;

	switch ( clientState ) {
	case kVERTEX_ARRAY:

		stateToDisable = GL_VERTEX_ARRAY;
		break;
	case kCOLOR_ARRAY:

		stateToDisable = GL_COLOR_ARRAY;
		break;
	case kTEXTURE_COORD_ARRAY:

		stateToDisable = GL_TEXTURE_COORD_ARRAY;
		break;
	default:
		break;
	} // end switch

	glDisableClientState( stateToDisable );
}

void CBRenderer::enableCullFace() const {

	glEnable( GL_CULL_FACE );
}


void CBRenderer::disableCullFace() const {

	glDisable( GL_CULL_FACE );
}


void CBRenderer::enableLighting() const {

	glEnable( GL_LIGHTING );
}

void CBRenderer::disableLighting() const {

	glDisable( GL_LIGHTING );
}

void CBRenderer::bindVBOBuffer( unsigned int VBOBufferID ) const {

	glBindBuffer( GL_ARRAY_BUFFER, VBOBufferID );
}


void CBRenderer::setVertexPointer( unsigned int size, cbengine::DataType dataType, unsigned int stride, void * pointer ) const {

	unsigned int dataTypeConversion = 0;
	switch ( dataType ) {
	case kTYPE_FLOAT:
		dataTypeConversion = GL_FLOAT;
		break;
	case kTYPE_DOUBLE:
		dataTypeConversion = GL_DOUBLE;
		break;
	case kTYPE_INT:
		dataTypeConversion = GL_INT;
		break;
	default:
		break;
	}

	glVertexPointer( size, dataTypeConversion, stride, pointer );
}


void CBRenderer::setColorPointer( unsigned int size, cbengine::DataType dataType, unsigned int stride, void * pointer ) const {

	unsigned int dataTypeConversion = 0;
	switch ( dataType ) {
	case kTYPE_FLOAT:
		dataTypeConversion = GL_FLOAT;
		break;
	case kTYPE_DOUBLE:
		dataTypeConversion = GL_DOUBLE;
		break;
	case kTYPE_INT:
		dataTypeConversion = GL_INT;
		break;
	default:
		break;
	}

	glColorPointer( size, dataTypeConversion, stride, pointer );

}


void CBRenderer::setTexCoordPointer( unsigned int size, cbengine::DataType dataType, unsigned int stride, void * pointer ) const {

	unsigned int dataTypeConversion = 0;
	switch ( dataType ) {
	case kTYPE_FLOAT:
		dataTypeConversion = GL_FLOAT;
		break;
	case kTYPE_DOUBLE:
		dataTypeConversion = GL_DOUBLE;
		break;
	case kTYPE_INT:
		dataTypeConversion = GL_INT;
		break;
	default:

		break;
	}

	glTexCoordPointer( size, dataTypeConversion, stride, pointer );	
}


void CBRenderer::drawArrays( cbengine::DrawPrimitive drawPrim, unsigned int start, unsigned int size ) const {

	unsigned int drawPrimitive = 0;
	switch ( drawPrim ) {
	case kPOINT:
		drawPrimitive = GL_POINT;
		break;
	case kQUADS:
		drawPrimitive = GL_QUADS;
		break;
	case kLINE_LOOP:
		drawPrimitive = GL_LINE_LOOP;
		break;
	case kTRIANGLES:
		drawPrimitive = GL_TRIANGLES;
		break;
	default:
		break;
	}
	
	glDrawArrays( drawPrimitive, start, size );
}


void CBRenderer::cullFaceBack() const {

	glCullFace( GL_BACK );
}


void CBRenderer::cullFrontFaceCounterClockwise() const {

	glFrontFace( GL_CCW );
}


void CBRenderer::enableDepthBuffering() const {

	glEnable( GL_DEPTH_TEST );
}

void CBRenderer::disableDepthBuffering() const {

	glDisable( GL_DEPTH_TEST );
}

void CBRenderer::generateVBOBuffers( unsigned int numBuffers, unsigned int * vboBufferID ) const {

	glGenBuffers( numBuffers, vboBufferID );
}

void CBRenderer::bufferDataForVBO( unsigned int sizeOfData, void * data ) const {

	glBufferData( GL_ARRAY_BUFFER, sizeOfData, data, GL_STATIC_DRAW );
}


void CBRenderer::deleteVBOBuffers( unsigned int numBuffers, unsigned int * vboBufferID ) const {

	glDeleteBuffers( numBuffers, vboBufferID );
}

void CBRenderer::setPerspective(  double fovy, double aspect, double zNear, double zFar ) const {

	gluPerspective( fovy , aspect , zNear, zFar );
}