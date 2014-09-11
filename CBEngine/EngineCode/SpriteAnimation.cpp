
#include "SpriteAnimation.hpp"

#include <cassert>

#include "SpriteAnimationDefinition.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::SpriteAnimation;

SpriteAnimation::~SpriteAnimation() {

	

} // end Dtor

SpriteAnimation::SpriteAnimation( cbengine::SpriteAnimationDefinition * animDef ) :
	m_currentTexturePosition( cbengine::AABB( cbengine::Vector2( 0.0f, 0.0f)))

{

	assert( animDef != nullptr );

	m_animationDefinition = animDef;
	
	m_timeExistedInSeconds = 0.0f;

} // end Ctor

void SpriteAnimation::update(float deltaSeconds ) {

	m_timeExistedInSeconds += deltaSeconds;

} // end update

void SpriteAnimation::render() const {

	// TODO remove explosion dependencies
	const float halfWidth = cbengine::EXPLOSION_WIDTH * 0.50f;
	const float halfHeight = cbengine::EXPLOSION_HEIGHT * 0.50f;

	// Grab Texture ID from animation def
	// Get texture coordinates based on time from SpriteAnimDef
	cbengine::Texture * texture;
	bool hasExpired = m_animationDefinition->getTextureForAnimationOrExpire( m_timeExistedInSeconds, texture, m_currentTexturePosition );
	assert(texture != nullptr);

	if (!hasExpired) {

		glPushMatrix();

		glTranslatef( m_position.x, m_position.y, 0.0f);
		glRotatef( m_orientationDegrees, 0.0f, 0.0f, 1.0f );

		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, texture->getGLTextureID());
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f ); 

		// Remember texture is loaded with origin at top left
		glBegin( GL_QUADS ); {

			
			glTexCoord2f( m_currentTexturePosition.m_origin.x, 
				m_currentTexturePosition.m_origin.y + m_currentTexturePosition.m_height );
			glVertex2f( -halfWidth, -halfHeight );

			glTexCoord2f( m_currentTexturePosition.m_origin.x + m_currentTexturePosition.m_width,
				m_currentTexturePosition.m_origin.y + m_currentTexturePosition.m_height );
			glVertex2f( halfWidth, -halfHeight );

			glTexCoord2f( m_currentTexturePosition.m_origin.x + m_currentTexturePosition.m_width, 
				m_currentTexturePosition.m_origin.y );
			glVertex2f( halfWidth, halfHeight );

			glTexCoord2f(m_currentTexturePosition.m_origin.x, 
				m_currentTexturePosition.m_origin.y );
			glVertex2f( -halfWidth, halfHeight );
		
		} glEnd();

		glPopMatrix();

	} else {

		// Flag as dirty as it has expired
		m_dirty = hasExpired;

	} // end if

} // end render