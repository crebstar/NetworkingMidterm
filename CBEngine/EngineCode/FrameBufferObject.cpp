#include "FrameBufferObject.hpp"

#include "EngineCommon.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

FrameBufferObject::~FrameBufferObject() {

}


FrameBufferObject::FrameBufferObject( const cbengine::Size & windowSize ) {
	initializeFrameBufferObject( windowSize );
}

void FrameBufferObject::initializeFrameBufferObject( const cbengine::Size & windowSize ) {

	// Create color for framebuffer texture
	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 );
	glGenBuffers( 1, &m_frameBufferColorTexture );
	glBindTexture( GL_TEXTURE_2D, m_frameBufferColorTexture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, static_cast<size_t>( windowSize.m_width ), 
		static_cast<size_t>( windowSize.m_height ), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

	// Create Depth Buffer Texture
	glActiveTexture( GL_TEXTURE4 );
	glGenTextures( 1, &m_frameBufferDepthTexture );
	glBindTexture( GL_TEXTURE_2D, m_frameBufferDepthTexture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast<size_t>( windowSize.m_width ), 
		static_cast<size_t>( windowSize.m_height ), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );

	// Create an FBO (Framebuffer Object) and activate it
	glGenFramebuffers( 1, &m_frameBufferObjectID );
	glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferObjectID );

	// Attach our color and depth textures to the FBO, in the color0 and depth FBO "slots"
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameBufferColorTexture, 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_frameBufferDepthTexture, 0);

	// Switch to the default framebuffer ;; User must toggle when to use it or not after construction
	glBindFramebuffer( GL_FRAMEBUFFER, 0 ); 

}

