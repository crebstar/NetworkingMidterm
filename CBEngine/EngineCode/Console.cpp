#include "Console.hpp"

#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "EngineCommon.hpp"
#include "TextureManager.hpp"
#include "CommandPrompt.hpp"
#include "BitmapFont.hpp"
#include "Glyph.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Console::~Console() {

	if ( m_commandPrompt ) {
		delete m_commandPrompt;
	}

	for ( size_t i = 0; i < m_consoleLines.size(); ++i ) {
		delete m_consoleLines[i];
	}
}


void Console::buildConsoleAndCommandPrompt( const std::string & consoleTextureName, 
	const std::string & commandPromptTextureName, 
	const cbengine::Vector4<float> & backgroundColorRGBA, 
	const cbengine::Vector2 & screenPositionCommandPrompt, 
	const cbengine::Vector2 & screenPositionConsole, 
	BitmapFont * bitmapFont )
{

	cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	cbengine::Texture * consoleBackgroundTexture = sharedTextureManager->generateOrGrabExistingTexture( consoleTextureName );
	assert( consoleBackgroundTexture != nullptr );
	m_texture = consoleBackgroundTexture;

	m_backgroundColorRGBA.x = backgroundColorRGBA.x;
	m_backgroundColorRGBA.y = backgroundColorRGBA.y;
	m_backgroundColorRGBA.z = backgroundColorRGBA.z;
	m_backgroundColorRGBA.w = backgroundColorRGBA.w;
	m_screenPosition.x = screenPositionConsole.x;
	m_screenPosition.y = screenPositionConsole.y;
	m_consoleFontHeight = CONSOLE_FONT_HEIGHT;

	createVBODataForBackground();
	assert( bitmapFont != nullptr );
	m_consoleFont = bitmapFont;
	m_commandPrompt = new CommandPrompt( screenPositionCommandPrompt, commandPromptTextureName, backgroundColorRGBA, bitmapFont );
	m_commandPrompt->m_promptDelegate = this;

}


void Console::update( float deltaSeconds ) {

	m_commandPrompt->update( deltaSeconds );
}


void Console::render( float deltaSeconds ) const {

	UNUSED(deltaSeconds);
	if ( !m_isVisible ) {
		return;
	}

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->disableDepthBuffering();

	sharedRenderer->pushMatrix();

	glTranslatef( m_screenPosition.x, m_screenPosition.y, 0.0f );

	sharedRenderer->bindVBOBuffer( m_vboBackgroundID );

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
	//sharedRenderer->enableDepthBuffering();

	glActiveTexture( GL_TEXTURE5 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE4 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE3 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE2 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE1 );
	sharedRenderer->bind2DTexture( 0 );
	glDisable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE0 );
	sharedRenderer->bind2DTexture( m_texture->getGLTextureID() );
	glEnable( GL_TEXTURE_2D );

	cbengine::DrawPrimitive drawPrim = cbengine::kQUADS;
	sharedRenderer->drawArrays( drawPrim, 0, m_backgroundVerts.size() );

	graphicsClientState = graphicsClientState = cbengine::kVERTEX_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kCOLOR_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kTEXTURE_COORD_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );

	sharedRenderer->disable2DTextures();

	sharedRenderer->popMatrix();

	renderConsoleText( deltaSeconds );

	m_commandPrompt->render( deltaSeconds );
}


void Console::renderConsoleText( float deltaSeconds ) const {

	UNUSED(deltaSeconds);
	if ( m_consoleLines.size() == 0 ) {
		return;
	}

	size_t lineCount = 0;
	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	float currentYScreenPosition = 900.0f - CONSOLE_HEIGHT + 20.0f; // TODO:: Remove this hardcoded 900 and 40 once happy with values
	for ( size_t i = ( m_consoleLines.size() - 1 ); i >= 0; --i ) {

		++lineCount;
		ConsoleLine * line = m_consoleLines[i];
		sharedRenderer->pushMatrix();
		glTranslatef( 0.0f, currentYScreenPosition, 0.0f );
		sharedRenderer->bindVBOBuffer( line->m_VBOID );

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

		sharedRenderer->disableLighting();
		sharedRenderer->enable2DTextures();
		//sharedRenderer->enableDepthBuffering();

		glActiveTexture( GL_TEXTURE0 );
		sharedRenderer->bind2DTexture( m_consoleFont->m_texture->getGLTextureID() );

		cbengine::DrawPrimitive drawPrim = cbengine::kQUADS;
		sharedRenderer->drawArrays( drawPrim, 0, line->m_verts.size() );

		currentYScreenPosition += m_consoleFontHeight * 1.5f;

		sharedRenderer->popMatrix();

		if ( i == 0 || ( lineCount >= CONSOLE_MAX_LINES ) ) {
			break;
		}

	}
	
	cbengine::GraphicsClientState graphicsClientState = cbengine::kVERTEX_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kCOLOR_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kTEXTURE_COORD_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );

	sharedRenderer->disable2DTextures();
}

void Console::createVBODataForBackground() {
	// Background is a textured Quad

	cbengine::Vertex backgroundVertex;
	backgroundVertex.vertexColor.x = m_backgroundColorRGBA.x;
	backgroundVertex.vertexColor.y = m_backgroundColorRGBA.y;
	backgroundVertex.vertexColor.z = m_backgroundColorRGBA.z;
	backgroundVertex.vertexColor.w = m_backgroundColorRGBA.w;

	// Bottom Left
	backgroundVertex.vertexTextureCoords.x = 0.0f;
	backgroundVertex.vertexTextureCoords.y = 1.0f;
	backgroundVertex.vertexPosition.x = 0.0f;
	backgroundVertex.vertexPosition.y = 0.0f;
	backgroundVertex.vertexPosition.z = 0.0f;
	m_backgroundVerts.push_back( backgroundVertex );

	// Bottom Right
	backgroundVertex.vertexTextureCoords.x = 1.0f;
	backgroundVertex.vertexTextureCoords.y = 1.0f;
	backgroundVertex.vertexPosition.x = CONSOLE_WIDTH;
	backgroundVertex.vertexPosition.y = 0.0f;
	backgroundVertex.vertexPosition.z = 0.0f;
	m_backgroundVerts.push_back( backgroundVertex );

	// Top Right
	backgroundVertex.vertexTextureCoords.x = 1.0f;
	backgroundVertex.vertexTextureCoords.y = 0.0f;
	backgroundVertex.vertexPosition.x = CONSOLE_WIDTH;
	backgroundVertex.vertexPosition.y = CONSOLE_HEIGHT;
	backgroundVertex.vertexPosition.z = 0.0f;
	m_backgroundVerts.push_back( backgroundVertex );

	// Top Left
	backgroundVertex.vertexTextureCoords.x = 0.0f;
	backgroundVertex.vertexTextureCoords.y = 0.0f;
	backgroundVertex.vertexPosition.x = 0.0f;
	backgroundVertex.vertexPosition.y = CONSOLE_HEIGHT;
	backgroundVertex.vertexPosition.z = 0.0f;
	m_backgroundVerts.push_back( backgroundVertex );

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &m_vboBackgroundID );
	sharedRenderer->bindVBOBuffer( m_vboBackgroundID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_backgroundVerts.size() ), &m_backgroundVerts.front() );

}


void Console::addTextToConsole( const std::string & textToAdd, const cbengine::Vector4<float> & color ) {

	ConsoleLine * lineToAdd = new ConsoleLine( color );
	float currentLineWidth = 0.0f;
	float XOffset = CONSOLE_X_MARGIN_OFFSET;
	float charWidth = 0.0f;

	for ( size_t i = 0; i < textToAdd.size(); ++i ) {

		std::map< unsigned char, Glyph * >::iterator it;
		it = m_consoleFont->m_fontGlyphs.find( textToAdd[i] );
		if ( it != m_consoleFont->m_fontGlyphs.end() ) {

			Glyph * glyph = it->second;
			const cbengine::Vector2 & minTexCoords = glyph->getMinTextureCoords();
			const cbengine::Vector2 & maxTexCoords = glyph->getMaxTextureCoords();
			XOffset += m_consoleFontHeight * glyph->getTTFA();
			charWidth = m_consoleFontHeight * glyph->getTTFB();

			// Create the verts
			cbengine::Vertex textVert;
			textVert.vertexColor.x = color.x;
			textVert.vertexColor.y = color.y;
			textVert.vertexColor.z = color.z;
			textVert.vertexColor.w = color.w;
			// Bottom Left
			textVert.vertexTextureCoords.x = minTexCoords.x;
			textVert.vertexTextureCoords.y = maxTexCoords.y;
			textVert.vertexPosition.x = XOffset;
			textVert.vertexPosition.y = 0.0f;
			textVert.vertexPosition.z = 0.0f;
			lineToAdd->m_verts.push_back( textVert );
			// Bottom Right
			textVert.vertexTextureCoords.x = maxTexCoords.x;
			textVert.vertexTextureCoords.y = maxTexCoords.y;
			textVert.vertexPosition.x = XOffset + charWidth;
			textVert.vertexPosition.y = 0.0f;
			textVert.vertexPosition.z = 0.0f;
			lineToAdd->m_verts.push_back( textVert );
			// Top Right
			textVert.vertexTextureCoords.x = maxTexCoords.x;
			textVert.vertexTextureCoords.y = minTexCoords.y;
			textVert.vertexPosition.x = XOffset + charWidth;
			textVert.vertexPosition.y = m_consoleFontHeight;
			textVert.vertexPosition.z = 0.0f;
			lineToAdd->m_verts.push_back( textVert );
			// Top Left
			textVert.vertexTextureCoords.x = minTexCoords.x;
			textVert.vertexTextureCoords.y = minTexCoords.y;
			textVert.vertexPosition.x = XOffset;
			textVert.vertexPosition.y = m_consoleFontHeight;
			textVert.vertexPosition.z = 0.0f;
			lineToAdd->m_verts.push_back( textVert );

			XOffset += m_consoleFontHeight * glyph->getTTFC();
			XOffset += charWidth;
			currentLineWidth = XOffset;
			lineToAdd->m_lineText += textToAdd[i];
			// TODO:: AVOID SPLITTING WORDS IN HALF ONTO NEW LINES

			if ( currentLineWidth > CONSOLE_MAX_LINE_WIDTH ) {
				// TODO

			} // end if
		} // end if
	} // end for

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &lineToAdd->m_VBOID );
	sharedRenderer->bindVBOBuffer( lineToAdd->m_VBOID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * lineToAdd->m_verts.size() ), &lineToAdd->m_verts.front() );

	m_consoleLines.push_back( lineToAdd );

}


void Console::addStringToConsole( const std::string & stringToAdd, const cbengine::Vector4<float> & color ) {

	addTextToConsole( stringToAdd, color );
}


void Console::clearCommandHistory() {

	m_consoleLines.clear();
}