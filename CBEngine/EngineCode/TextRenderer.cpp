#include "TextRenderer.hpp"

#include <map>

#include "EngineCommon.hpp"
#include "BitmapFont.hpp"
#include "Glyph.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

TextRenderer::~TextRenderer() {

}


void TextRenderer::drawDebugText( const std::string & textToDraw, 
	const BitmapFont & font, 
	float cellHeight, 
	const cbengine::Vector2 & screenPosition, 
	const cbengine::Vector4<float> & textColorRGBA ) 
{

	glPushMatrix();

	glTranslatef( screenPosition.x, screenPosition.y, 0.0f );
	float xOffset = 0.0f;
	
	glEnable( GL_TEXTURE_2D );
	unsigned int textureID = font.m_texture->getGLTextureID();
	glBindTexture( GL_TEXTURE_2D, textureID );

	glColor4f( textColorRGBA.x, textColorRGBA.y, textColorRGBA.z, textColorRGBA.w );

	glBegin( GL_QUADS ); {

		for ( size_t i = 0; i < textToDraw.size(); ++i ) {
		
			std::map< unsigned char, Glyph * >::const_iterator it;
			it = font.m_fontGlyphs.find( textToDraw[i] );

			if ( it != font.m_fontGlyphs.end() ) {

				float charWidth = 0.0f;
				Glyph * glyph = it->second;
				const cbengine::Vector2 & minTexCoords = glyph->getMinTextureCoords();
				const cbengine::Vector2 & maxTexCoords = glyph->getMaxTextureCoords();
				xOffset += cellHeight * glyph->getTTFA();
				charWidth += cellHeight * glyph->getTTFB();
				

				glTexCoord2f( minTexCoords.x, maxTexCoords.y );
				glVertex2f( xOffset, 0.0f );
				
				glTexCoord2f( maxTexCoords.x, maxTexCoords.y );
				glVertex2f( xOffset + charWidth, 0.0f );
				
				glTexCoord2f( maxTexCoords.x, minTexCoords.y );
				glVertex2f( xOffset + charWidth, cellHeight );

				glTexCoord2f( minTexCoords.x, minTexCoords.y );
				glVertex2f( xOffset, cellHeight );

				xOffset += cellHeight * glyph->getTTFC();
				xOffset += charWidth;

			}

		}

	} glEnd();

	glPopMatrix();

}