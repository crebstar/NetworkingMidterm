#ifndef included_FrameBufferObject
#define included_FrameBufferObject
#pragma once

#include "Size.hpp"

#include "EngineCommon.hpp"

class FrameBufferObject {
public:

	explicit FrameBufferObject( const cbengine::Size & windowSize );
	~FrameBufferObject();

	// Inline Functions
	void bindFrameBufferObject() const;
	void stopRenderingToFrameBuffer() const;

	// Inline Mutator Functions
	unsigned int getFrameBufferObjectID() const;
	unsigned int getFrameBufferColorTextureID() const;
	unsigned int getFrameBufferDepthTextureID() const;
	// Use this function to know if we are currently rendering to this FBO
	bool		 isRenderingToFrameBuffer() const;

protected:


private:

	void initializeFrameBufferObject( const cbengine::Size & windowSize );

	// Poll this variable to check if currently FBO is in use
	mutable bool		m_renderingToFrameBuffer;

	unsigned int		m_frameBufferObjectID;
	unsigned int		m_frameBufferColorTexture;
	unsigned int		m_frameBufferDepthTexture;

};


inline void FrameBufferObject::bindFrameBufferObject() const {
	glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferObjectID );
	m_renderingToFrameBuffer = true;
}


inline void FrameBufferObject::stopRenderingToFrameBuffer() const {
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	m_renderingToFrameBuffer = false;
}

// Inline Mutator functions 
inline unsigned int FrameBufferObject::getFrameBufferObjectID() const {
	return m_frameBufferObjectID;
}


inline unsigned int FrameBufferObject::getFrameBufferColorTextureID() const {
	return m_frameBufferColorTexture;
}


inline unsigned int FrameBufferObject::getFrameBufferDepthTextureID() const {
	return m_frameBufferDepthTexture;
}


inline bool FrameBufferObject::isRenderingToFrameBuffer() const {
	return m_renderingToFrameBuffer;
}

/*
/////////////////////////////////////////////////////////////////////////////
// FRAMEBUFFER OBJECT INITIALIZATION (RUN ONCE AT STARTUP)
/////////////////////////////////////////////////////////////////////////////
GLuint g_frameBufferObject = 0;
GLuint g_framebufferColorTexture = 0;
GLuint g_framebufferDepthTexture = 0;

// Create color framebuffer texture
glActiveTexture( GL_TEXTURE0 );
glGenTextures( 1, &g_framebufferColorTexture );
glBindTexture( GL_TEXTURE_2D, g_framebufferColorTexture );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

// Create depth framebuffer texture
glActiveTexture( GL_TEXTURE1 );
glGenTextures( 1, &g_framebufferDepthTexture );
glBindTexture( GL_TEXTURE_2D, g_framebufferDepthTexture );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );

// Create an FBO (Framebuffer Object) and activate it
glGenFramebuffers( 1, &g_frameBufferObject );
glBindFramebuffer( GL_FRAMEBUFFER, g_frameBufferObject );

// Attach our color and depth textures to the FBO, in the color0 and depth FBO "slots"
glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_framebufferColorTexture, 0 );
glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g_framebufferDepthTexture, 0);


/////////////////////////////////////////////////////////////////////////////
// DRAW YOUR 3D SCENE INTO YOUR CUSTOM FBO (RUN ONCE PER FRAME)
/////////////////////////////////////////////////////////////////////////////
glBindFramebuffer( GL_FRAMEBUFFER, g_frameBufferObject );
DrawYour3DSceneAsNormal(); // This is now automatically drawn into the FBO, rather than the actual framebuffer
glBindFramebuffer( GL_FRAMEBUFFER, 0 );


/////////////////////////////////////////////////////////////////////////////
// DRAW YOUR FBO ONTO THE SCREEN WITH A CUSTOM FRAMEBUFFER EFFECT (RUN ONCE NEAR END OF EACH FRAME)
/////////////////////////////////////////////////////////////////////////////
glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // Not drawing to the FBO anymore; now we're rendering to framebuffer for real
//Prepare for simple 2D ortho drawing:
glClearColor( 0.35f, 0.35f, 0.35f, 1.f );
glClearDepth( 1.f );
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glDisable( GL_DEPTH_TEST );
glDepthMask( GL_FALSE );
glOrtho( 0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0 ); // or equivalent in your own matrix stack
glUseProgram( myFramebufferEffectShaderProgramID ); // Use a totally different shader program to perform your framebuffer effects

// Bind your two FBO textures now as INPUTS into the framebuffer effect shader, and pass them in (as uniforms, per usual):
glActiveTexture( GL_TEXTURE1 );
glBindTexture( GL_TEXTURE_2D, g_framebufferDepthTexture );
glActiveTexture( GL_TEXTURE0 );
glBindTexture( GL_TEXTURE_2D, g_framebufferColorTexture );
glUniform1i( framebufferColorTextureUniformLocation, 0 ); // Use the already-rendered scene's color data as a simple texture, as you please
glUniform1i( framebufferDepthTextureUniformLocation, 1 ); // Use the already-rendered scene's depth data as a simple texture, as you please
glBegin() or glDrawElements()... // Draw a single fullscreen quad

glUseProgram( 0 ); // Turn off your framebuffer effect shader program



*/

#endif