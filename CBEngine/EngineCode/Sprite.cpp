


#include "Sprite.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "TextureManager.hpp"
#include "Texture.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::Sprite;


Sprite::~Sprite() {



} // end dtor


Sprite::Sprite() {

	m_texture = nullptr;

} // end ctor


Sprite::Sprite( const std::string & spriteTextureFileName ) {

	initWithTexture( spriteTextureFileName );


}

void Sprite::update( float deltaSeconds ) {
	UNUSED(deltaSeconds);
}

void Sprite::render() const {

	drawBasedOnGLTexture();

}

// TODO change radius settings
void Sprite::initWithTexture( const std::string & textureFileName ) {

	cbengine::TextureManager * textureManager = cbengine::TextureManager::sharedTextureManager(); 

	Texture * spriteTexture = textureManager->generateOrGrabExistingTexture( textureFileName );

	if (spriteTexture != nullptr ) {

		m_texture = spriteTexture;
		
	} else {
		// Error loading texture
		m_texture = nullptr;
	}

}


void Sprite::drawBasedOnGLTexture() const {

	if (m_texture == nullptr) return;

	const float halfWidth = m_ContentSize.m_width * 0.50f;
	const float halfHeight = m_ContentSize.m_height * 0.50f;
	
	glPushMatrix();

	glTranslatef( m_position.x, m_position.y, 0.0f );
	glRotatef( m_orientationDegrees, 0.0f, 0.0f, 1.0f );
	

	glEnable( GL_TEXTURE_2D );
	unsigned int glTextureID = m_texture->getGLTextureID();
	glBindTexture( GL_TEXTURE_2D, glTextureID );
	glBegin( GL_QUADS ); {

		glTexCoord2f( 0.0f, 1.0f);
		glVertex2f( -halfWidth, -halfHeight);

		glTexCoord2f( 1.0f, 1.0f);
		glVertex2f( halfWidth, -halfHeight);

		glTexCoord2f( 1.0f, 0.0f);
		glVertex2f( halfWidth, halfHeight);

		glTexCoord2f( 0.0, 0.0);
		glVertex2f( -halfWidth, halfHeight);

	} glEnd();

	glPopMatrix();


} // end drawBasedOnGLTexture
