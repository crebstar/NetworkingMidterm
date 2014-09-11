
#ifndef included_CBRenderer
#define included_CBRenderer
#pragma once

#include "..\EngineCode\EngineMacros.hpp"
#include "..\EngineCode\Vector2.hpp"
#include "..\EngineCode\Vector3D.hpp"
#include "..\EngineCode\Vector4.hpp"

namespace cbengine {

typedef enum {

	kPOINT,
	kQUADS,
	kLINE_LOOP,
	kTRIANGLES,

} DrawPrimitive; 

typedef enum {

	kVERTEX_ARRAY,
	kCOLOR_ARRAY,
	kTEXTURE_COORD_ARRAY,

} GraphicsClientState;

typedef enum {

	kUNKNOWN,
	kOPEN_GL,
	kDIRECT_X,

} GraphicsEngineAPI;

typedef enum {

	kTYPE_FLOAT,
	kTYPE_INT,
	kTYPE_DOUBLE,

} DataType;

class CBRenderer {
public:

	static CBRenderer* sharedCBRenderer() {

		static CBRenderer sharedCBRenderer;
		return &sharedCBRenderer;
	} // end singleton share method

	~CBRenderer();

	// Fixed Function Graphics API Functions
	void pushMatrix() const;
	void popMatrix() const;
	void enable2DTextures() const;
	void disable2DTextures() const;
	void bind2DTexture( unsigned int textureID ) const;
	void color3F( const Vector3<float> & colorValues ) const;
	void color4F( const Vector4<float> & colorValues ) const;
	void translateF( const Vector3<float> & translatePosition ) const;
	void rotateF( float orientationDegrees, float x, float y, float z ) const;
	void vertex3F( float x, float y, float z ) const;
	void texCoords2F( float s, float t ) const;
	void beginDraw( DrawPrimitive drawPrim ) const;
	void endDraw() const;
	void setLineWidth( float widthOfLine ) const;

	void clearBuffer();

	void enableClientState( GraphicsClientState clientState ) const;
	void disableClientState( GraphicsClientState clientState ) const;
	void enableCullFace() const;
	void disableCullFace() const;
	void cullFrontFaceCounterClockwise() const;
	void cullFaceBack() const;
	void enableLighting() const;
	void disableLighting() const;
	void enableDepthBuffering() const;
	void disableDepthBuffering() const;

	void setVertexPointer( unsigned int size, DataType dataType, unsigned int stride, void * pointer ) const;
	void setColorPointer( unsigned int size, DataType dataType, unsigned int stride, void * pointer ) const;
	void setTexCoordPointer( unsigned int size, DataType dataType, unsigned int stride, void * pointer ) const;

	void drawArrays( DrawPrimitive drawPrim, unsigned int start, unsigned int size ) const;

	void generateVBOBuffers( unsigned int numBuffers, unsigned int * vboBufferID ) const;
	void bindVBOBuffer( unsigned int VBOBufferID ) const;
	void bufferDataForVBO( unsigned int sizeOfData, void * data ) const;
	void deleteVBOBuffers( unsigned int numBuffers, unsigned int * vboBufferID ) const;	

	void setPerspective( double fovy, double aspect, double zNear, double zFar ) const;

	void setShouldClearDepthBuffer( bool shouldClearDepthBuffer );
protected:

	CBRenderer() {

		determineCurrentGraphicsAPIToUse();
		m_shouldClearDepthBuffer = false;
	}

private:
	PREVENT_COPY_AND_ASSIGN( CBRenderer );

	// Private Functions
	void determineCurrentGraphicsAPIToUse();

	// Private Variables
	GraphicsEngineAPI currentGraphicsAPI;

	bool			m_shouldClearDepthBuffer;
};


// Inline Functions
inline void CBRenderer::determineCurrentGraphicsAPIToUse() {

	// For now...
	currentGraphicsAPI = kOPEN_GL;
}


inline void CBRenderer::setShouldClearDepthBuffer( bool shouldClearDepthBuffer ) {

	m_shouldClearDepthBuffer = shouldClearDepthBuffer;
}

} // end namespace

#endif