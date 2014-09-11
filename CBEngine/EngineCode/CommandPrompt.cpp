#include "CommandPrompt.hpp"

#include <cassert>
#include <math.h>

#include "EngineCommon.hpp"
#include "MathUtil.hpp"
#include "TextureManager.hpp"
#include "BitmapFont.hpp"
#include "Glyph.hpp"
#include "Console.hpp"
#include "InputHandler.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CommandPrompt::~CommandPrompt() {

}

CommandPrompt::CommandPrompt( const cbengine::Vector2 & screenPosition, 
	const std::string & backgroundTextureName,
	const cbengine::Vector4<float> & backgroundColorRGBA,
	BitmapFont * bitmapFont ) 
{

	m_promptDelegate = nullptr;
	m_commandPromptText = "";
	m_indexOffsetFromEndOfCPText = 0;
	m_screenPosition.x = screenPosition.x;
	m_screenPosition.y = screenPosition.y;
	m_consoleFontHeight = COMMAND_PROMPT_FONT_SIZE;
	m_commandTextVBOIDGenerated = false;

	cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	cbengine::Texture * backgroundTexture = sharedTextureManager->generateOrGrabExistingTexture(  backgroundTextureName );
	assert( backgroundTexture != nullptr );
	m_texture = backgroundTexture;

	m_backgroundColorRGBA.x = backgroundColorRGBA.x;
	m_backgroundColorRGBA.y = backgroundColorRGBA.y;
	m_backgroundColorRGBA.z = backgroundColorRGBA.z;
	m_backgroundColorRGBA.w = backgroundColorRGBA.w;
	assert( bitmapFont != nullptr );
	m_bitmapFont = bitmapFont;

	setUpVertexDataForVBO();
	setUpVertexDataForStartLineAndCursorVBOS();

	m_startCharPosition.x = 5.0f;
	m_startCharPosition.y = 10.0f;
	m_cursorCharPosition.x = 40.0f;
	m_cursorCharPosition.y = 10.0f;

	registerBuiltInCommands();

	m_keysDownLastFrame = new bool[256];
	for( size_t i = 0; i < 256; ++i ) {
		m_keysDownLastFrame[i] = false;
	}

	m_commandHistoryIndex = 0;

}


void CommandPrompt::update( float deltaSeconds ) {

	Console* sharedConsole = Console::getSharedDeveloperConsole();
	bool isConsoleVisible = sharedConsole->isConsoleVisible();

	if ( isConsoleVisible ) {
		// Process Input
		processKeyboardInput( deltaSeconds );
	}

	rebuildCommandPromptStringVBO();
	determineCurrentPositionOfCursor();
	rebuildCursorVBO( deltaSeconds );
}


void CommandPrompt::determineCurrentPositionOfCursor() {
	// Guarenteed to always be atleast this
	float XOffset = textStartPosition.x;
	if ( m_commandPromptText != "" ) {
		std::string textToCheckWidth;

		for ( size_t i = 0; i < ( m_commandPromptText.size() - m_indexOffsetFromEndOfCPText ); ++i ) {
			textToCheckWidth += m_commandPromptText[i];
		}
		float widthOfText = m_bitmapFont->calculateTextWidth( textToCheckWidth, COMMAND_PROMPT_FONT_SIZE );
		XOffset += widthOfText;
	}
	m_cursorCharPosition.x = XOffset;

}

void CommandPrompt::processKeyboardInput( float deltaSeconds ) {
	UNUSED(deltaSeconds);
	// Special Keys are
	// Left and right arrow, Shift
	// Backspace, Enter, Escape
	InputHandler& sharedInputHandler = InputHandler::getSharedInputHandler();
	const bool * keyboardStatus = sharedInputHandler.getKeyboardStatus();
	std::queue<unsigned char> & wmCharQueue = sharedInputHandler.getWmCharMessageQueue();

	static bool enterKeyDown = false;
	if ( keyboardStatus[ VK_RETURN ] ) {
		// Enter ( Process currently entered text )
		if ( !enterKeyDown ) {

			if ( m_commandPromptText != "" ) {
				processCurrentCommandPromptString();
			} else {
				Console* sharedConsole = Console::getSharedDeveloperConsole();
				sharedConsole->setConsoleVisible( false );
			}

			m_commandPromptText = "";
			m_indexOffsetFromEndOfCPText = 0;
		}
		enterKeyDown = true;
	} else {
		enterKeyDown = false;
	}


	static bool backspaceKeyDown = false;
	if ( keyboardStatus[ VK_BACK ] ) {
		if ( !backspaceKeyDown && m_commandPromptText != "" && ( m_commandPromptText.size() != static_cast< unsigned int >( m_indexOffsetFromEndOfCPText ) ) ) {
			//m_commandPromptText = m_commandPromptText.substr(0, (m_commandPromptText.size()-1) - (m_indexOffsetFromEndOfCPText-1) );
			m_commandPromptText.erase( ( m_commandPromptText.size() - 1 ) - ( m_indexOffsetFromEndOfCPText ), 1 );
		}
		backspaceKeyDown = true;
	} else {
		backspaceKeyDown = false;
	}
	
	// TODO
	static bool escapeKeyDown = false;
	if ( keyboardStatus[ VK_ESCAPE ] ) {
		// Escape :: IF empty then dismiss
		if ( !escapeKeyDown ) {
			if ( m_commandPromptText == "" ) {

				Console* sharedConsole = Console::getSharedDeveloperConsole();
				sharedConsole->setConsoleVisible( false );
			} else {

				m_commandPromptText = "";
				m_indexOffsetFromEndOfCPText = 0;
			}
		}

		escapeKeyDown = true;
	} else {
		escapeKeyDown = false;
	}

	static bool homeKeyDown = false;
	if ( keyboardStatus[ VK_HOME ] ) {
		if ( !homeKeyDown ) {
			m_indexOffsetFromEndOfCPText = m_commandPromptText.size();
		}
		homeKeyDown = true;

	} else {
		homeKeyDown = false;
	}

	static bool endKeyDown = false;
	if ( keyboardStatus[ VK_END ] ) {

		if ( !endKeyDown ) {
			m_indexOffsetFromEndOfCPText = 0;
		}

		endKeyDown = true;
	} else {
		endKeyDown = false;
	}

	static bool upArrowDown = false;
	if ( keyboardStatus[ VK_UP ] ) {
		if ( !upArrowDown && m_commandHistory.size() != 0 ) {
			m_commandPromptText = m_commandHistory[m_commandHistoryIndex];
			--m_commandHistoryIndex;
			if ( m_commandHistoryIndex < 0 ) {
				m_commandHistoryIndex = m_commandHistory.size() - 1;
			}
		}

		upArrowDown = true;
	} else {
		upArrowDown = false;
	}

	static bool downArrowDown = false;
	if ( keyboardStatus[ VK_DOWN ] ){
		if ( !downArrowDown && m_commandHistory.size() != 0 ) {
			m_commandHistoryIndex++;
			if ( m_commandHistoryIndex > static_cast<int>(m_commandHistory.size() - 1) ) {
				m_commandHistoryIndex = 0;
			}
			m_commandPromptText = m_commandHistory[ m_commandHistoryIndex ];
		}
		downArrowDown = true;
	} else {
		downArrowDown = false;
	}

	static bool deleteKeyDown = false;
	if ( keyboardStatus[ VK_DELETE ] ) {
		if ( !deleteKeyDown ) {
			if ( m_indexOffsetFromEndOfCPText != 0 && m_commandPromptText != "" ) {
				m_commandPromptText.erase( ( m_commandPromptText.size() - 1 ) - ( m_indexOffsetFromEndOfCPText ) + 1, 1 );
				--m_indexOffsetFromEndOfCPText;
			}
		}
		deleteKeyDown = true;
	} else {
		deleteKeyDown = false;
	}

	static bool leftArrowDown = false;
	if ( keyboardStatus[ VK_LEFT ] ) {
		// Left Arrow :: Move cursor one to left
		if ( !leftArrowDown ) {
			++m_indexOffsetFromEndOfCPText;
			if ( m_indexOffsetFromEndOfCPText > static_cast<int>( m_commandPromptText.size() ) ) {
				m_indexOffsetFromEndOfCPText = m_commandPromptText.size();
			}
		}
		
		leftArrowDown = true;
		return;
	} else {
		leftArrowDown = false;
	}
	
	static bool rightArrowDown = false;
	if ( keyboardStatus[ VK_RIGHT ] ) {
		// Right Arrow :: Move cursor one to right
		if ( !rightArrowDown ) {
			--m_indexOffsetFromEndOfCPText;
			if ( m_indexOffsetFromEndOfCPText < 0 ) {
				m_indexOffsetFromEndOfCPText = 0;
			}
		}
		rightArrowDown = true;
		return;
	} else {
		rightArrowDown = false;
	}

	while ( !wmCharQueue.empty() ) {
		unsigned char keyDown = static_cast< unsigned char >( wmCharQueue.front() );
		wmCharQueue.pop();

		if ( keyDown == '`' || keyDown == '~' ) {
			continue;
		}

		std::map< unsigned char, Glyph * >::iterator it;
		it = m_bitmapFont->m_fontGlyphs.find( keyDown );
		if ( it != m_bitmapFont->m_fontGlyphs.end() ) {
			insertCharIntoCPString( keyDown );
		}
	}

}


void CommandPrompt::insertCharIntoCPString( char charToInsert ) {

	if ( m_indexOffsetFromEndOfCPText == 0 ) {
		m_commandPromptText += charToInsert;
	} else {
		m_commandPromptText.insert( ( m_commandPromptText.size() - m_indexOffsetFromEndOfCPText ), 1, charToInsert );
	}

}


void CommandPrompt::processCurrentCommandPromptString() {

	if ( m_promptDelegate != nullptr ) {
		ConsoleCommandArg args;
		convertCurrentCommandToCommandArgs( args );

		bool commandFound = false;
		std::map< std::string, CommandFuncPtr >::iterator it;
		for ( it = m_commandOptions.begin(); it != m_commandOptions.end(); ++it ) {

			const std::string & commandKey = it->first;
			
			if ( commandKey == args.m_mainCommand ) {
				CommandFuncPtr functToCall = it->second;
				functToCall( args );
				commandFound = true;
			}
			
		}

		m_commandHistory.push_back( m_commandPromptText );
		m_commandHistoryIndex = m_commandHistory.size() - 1;

		if ( commandFound ) {
			/* Don't like this feature
			if ( args.m_mainCommand != CLEAR_COMMAND_LOWER || args.m_mainCommand != CLEAR_COMMAND_UPPER ||
				args.m_mainCommand != LOG_COMMAND_LOWER || args.m_mainCommand != LOG_COMMAND_UPPER ) {
				cbengine::Vector4<float> color( 0.10f, 0.95f, 0.15f, 1.0f );
				m_promptDelegate->addStringToConsole( m_commandPromptText, color );
			}
			*/
		} else {
			std::string errorMessage = "ERROR: '";
			errorMessage += m_commandPromptText;
			errorMessage += "' is not a recognized command.";
			cbengine::Vector4<float> color( 0.95f, 0.20f, 0.15f, 1.0f );
			m_promptDelegate->addStringToConsole( errorMessage, color );
		}

	}
}

void CommandPrompt::convertCurrentCommandToCommandArgs( ConsoleCommandArg & args ) {
	args.m_fullCommand = m_commandPromptText;
	std::string mainArg = "";
	size_t currentIndex = 0;
	for ( size_t i = 0; i < m_commandPromptText.size(); ++i ) {
		if ( m_commandPromptText[i] == ' ' ) {
			currentIndex = (i+1);
			break;
		}
		mainArg += m_commandPromptText[i];
	}
	args.m_mainCommand = mainArg;

	std::string additionalArg = "";
	for ( size_t i = currentIndex; i < m_commandPromptText.size(); ++i ) {
		if ( m_commandPromptText[i] == ' ' ) {
			args.m_commandArgs.push_back( additionalArg );
			additionalArg = "";
			continue;
		}
		additionalArg += m_commandPromptText[i];
	}
	args.m_commandArgs.push_back( additionalArg );
}


void CommandPrompt::render( float deltaSeconds ) const {

	UNUSED(deltaSeconds);
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

	sharedRenderer->enable2DTextures();
	glActiveTexture( GL_TEXTURE0 );
	sharedRenderer->bind2DTexture( m_texture->getGLTextureID() );

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

	renderCommandPromptText();
	renderStartLineChar();
	renderCursorChar();
}


void CommandPrompt::renderCommandPromptText() const {
	// No need to render if the string is empty
	if ( m_commandPromptText == "" ) {
		return;
	}
	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->pushMatrix();
	// For now
	glTranslatef( textStartPosition.x, textStartPosition.y, 0.0f );

	sharedRenderer->bindVBOBuffer( m_cpTextVBOID );

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
	sharedRenderer->bind2DTexture( m_bitmapFont->m_texture->getGLTextureID() );

	cbengine::DrawPrimitive drawPrim = cbengine::kQUADS;
	sharedRenderer->drawArrays( drawPrim, 0, m_cPTextVerts.size() );
	
	graphicsClientState = graphicsClientState = cbengine::kVERTEX_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kCOLOR_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kTEXTURE_COORD_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );

	sharedRenderer->disable2DTextures();

	sharedRenderer->popMatrix();
}

void CommandPrompt::renderStartLineChar() const {
	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();

	sharedRenderer->pushMatrix();

	glTranslatef( m_startCharPosition.x, m_startCharPosition.y, 0.0f );

	sharedRenderer->bindVBOBuffer( m_startLineCharVBOID );

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
	sharedRenderer->bind2DTexture( m_bitmapFont->m_texture->getGLTextureID() );

	cbengine::DrawPrimitive drawPrim = cbengine::kQUADS;
	sharedRenderer->drawArrays( drawPrim, 0, m_startLineCharVerts.size() );

	graphicsClientState = graphicsClientState = cbengine::kVERTEX_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kCOLOR_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kTEXTURE_COORD_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );

	sharedRenderer->disable2DTextures();

	sharedRenderer->popMatrix();

}


void CommandPrompt::renderCursorChar() const {
	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();

	sharedRenderer->pushMatrix();

	glTranslatef( m_cursorCharPosition.x, m_cursorCharPosition.y, 0.0f );

	sharedRenderer->bindVBOBuffer( m_cursorVBOID );

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
	sharedRenderer->bind2DTexture( m_bitmapFont->m_texture->getGLTextureID() );

	cbengine::DrawPrimitive drawPrim = cbengine::kQUADS;
	sharedRenderer->drawArrays( drawPrim, 0, m_cursorVerts.size() );

	graphicsClientState = graphicsClientState = cbengine::kVERTEX_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kCOLOR_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );
	graphicsClientState = cbengine::kTEXTURE_COORD_ARRAY;
	sharedRenderer->disableClientState( graphicsClientState );

	sharedRenderer->disable2DTextures();

	sharedRenderer->popMatrix();
}

void CommandPrompt::setUpVertexDataForVBO() {
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
	backgroundVertex.vertexPosition.x = COMMAND_PROMPT_WIDTH;
	backgroundVertex.vertexPosition.y = 0.0f;
	backgroundVertex.vertexPosition.z = 0.0f;
	m_backgroundVerts.push_back( backgroundVertex );

	// Top Right
	backgroundVertex.vertexTextureCoords.x = 1.0f;
	backgroundVertex.vertexTextureCoords.y = 0.0f;
	backgroundVertex.vertexPosition.x = COMMAND_PROMPT_WIDTH;
	backgroundVertex.vertexPosition.y = COMMAND_PROMPT_HEIGHT;
	backgroundVertex.vertexPosition.z = 0.0f;
	m_backgroundVerts.push_back( backgroundVertex );

	// Top Left
	backgroundVertex.vertexTextureCoords.x = 0.0f;
	backgroundVertex.vertexTextureCoords.y = 0.0f;
	backgroundVertex.vertexPosition.x = 0.0f;
	backgroundVertex.vertexPosition.y = COMMAND_PROMPT_HEIGHT;
	backgroundVertex.vertexPosition.z = 0.0f;
	m_backgroundVerts.push_back( backgroundVertex );

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &m_vboBackgroundID );
	sharedRenderer->bindVBOBuffer( m_vboBackgroundID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_backgroundVerts.size() ), &m_backgroundVerts.front() );

}


void CommandPrompt::setUpVertexDataForStartLineAndCursorVBOS() {

	const float charWidth = COMMAND_PROMPT_FONT_SIZE * 0.70f;
	const float charHeight = COMMAND_PROMPT_FONT_SIZE;
	cbengine::Vertex cursorVert;
	cbengine::Vertex startLineCharVert;
	startLineCharVert.vertexColor.x = startLineCharColor.x;
	startLineCharVert.vertexColor.y = startLineCharColor.y;
	startLineCharVert.vertexColor.z = startLineCharColor.z;
	startLineCharVert.vertexColor.w = startLineCharColor.w;
	cursorVert.vertexColor.x = cursorColor.x;
	cursorVert.vertexColor.y = cursorColor.y;
	cursorVert.vertexColor.z = cursorColor.z;
	cursorVert.vertexColor.w = cursorColor.w;

	std::map< unsigned char, Glyph * >::iterator itStart;
	std::map< unsigned char, Glyph * >::iterator itCursor;

	itStart = m_bitmapFont->m_fontGlyphs.find( CP_START_LINE_CHAR );
	itCursor = m_bitmapFont->m_fontGlyphs.find( CP_CURSOR_CHAR );
	
	Glyph * startGlyph = itStart->second;
	Glyph * cursorGlyph = itCursor->second;

	const cbengine::Vector2 & minTexCoordsStartChar = startGlyph->getMinTextureCoords();
	const cbengine::Vector2 & maxTexCoordsStartChar = startGlyph->getMaxTextureCoords();
	const cbengine::Vector2 & minTexCoordsCursorChar = cursorGlyph->getMinTextureCoords();
	const cbengine::Vector2 & maxTexCoordsCursorChar = cursorGlyph->getMaxTextureCoords();
	// Bottom left corner is anchor point
	// Bottom Left
	startLineCharVert.vertexTextureCoords.x = minTexCoordsStartChar.x;
	startLineCharVert.vertexTextureCoords.y = maxTexCoordsStartChar.y;
	cursorVert.vertexTextureCoords.x		= minTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= maxTexCoordsCursorChar.y;
	startLineCharVert.vertexPosition.x = 0.0f;
	startLineCharVert.vertexPosition.y = 0.0f;
	startLineCharVert.vertexPosition.z = 0.0f;
	cursorVert.vertexPosition.x = 0.0f;
	cursorVert.vertexPosition.y = 0.0f;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );
	m_startLineCharVerts.push_back( startLineCharVert );
	// Bottom Right
	startLineCharVert.vertexTextureCoords.x = maxTexCoordsStartChar.x;
	startLineCharVert.vertexTextureCoords.y = maxTexCoordsStartChar.y;
	cursorVert.vertexTextureCoords.x		= maxTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= maxTexCoordsCursorChar.y;
	startLineCharVert.vertexPosition.x = charWidth;
	startLineCharVert.vertexPosition.y = 0.0f;
	startLineCharVert.vertexPosition.z = 0.0f;
	cursorVert.vertexPosition.x = charWidth * 0.10f;
	cursorVert.vertexPosition.y = 0.0f;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );
	m_startLineCharVerts.push_back( startLineCharVert );
	// Top Right
	startLineCharVert.vertexTextureCoords.x = maxTexCoordsStartChar.x;
	startLineCharVert.vertexTextureCoords.y = minTexCoordsStartChar.y;
	cursorVert.vertexTextureCoords.x		= maxTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= minTexCoordsCursorChar.y;
	startLineCharVert.vertexPosition.x = charWidth;
	startLineCharVert.vertexPosition.y = charHeight;
	startLineCharVert.vertexPosition.z = 0.0f;
	cursorVert.vertexPosition.x = charWidth * 0.10f;
	cursorVert.vertexPosition.y = charHeight;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );
	m_startLineCharVerts.push_back( startLineCharVert );
	// Top Left
	startLineCharVert.vertexTextureCoords.x = minTexCoordsStartChar.x;
	startLineCharVert.vertexTextureCoords.y = minTexCoordsStartChar.y;
	cursorVert.vertexTextureCoords.x		= minTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= minTexCoordsCursorChar.y;
	startLineCharVert.vertexPosition.x = 0.0f;
	startLineCharVert.vertexPosition.y = charHeight;
	startLineCharVert.vertexPosition.z = 0.0f;
	cursorVert.vertexPosition.x = 0.0f;
	cursorVert.vertexPosition.y = charHeight;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );
	m_startLineCharVerts.push_back( startLineCharVert );

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->generateVBOBuffers( 1, &m_startLineCharVBOID );
	sharedRenderer->bindVBOBuffer( m_startLineCharVBOID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_startLineCharVerts.size() ), &m_startLineCharVerts.front() );
	sharedRenderer->generateVBOBuffers( 1, &m_cursorVBOID );
	sharedRenderer->bindVBOBuffer( m_cursorVBOID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_cursorVerts.size() ), &m_cursorVerts.front() );

}


void CommandPrompt::rebuildCommandPromptStringVBO() {

	float XOffset = 0.0f;
	float charWidth = 0.0f;
	m_cPTextVerts.clear();
	cbengine::Vertex textVert;
	textVert.vertexColor.x = cpTextColor.x;
	textVert.vertexColor.y = cpTextColor.y;
	textVert.vertexColor.z = cpTextColor.z;
	textVert.vertexColor.w = cpTextColor.w;
	for ( size_t i = 0; i < m_commandPromptText.size(); ++i ) {

		std::map< unsigned char, Glyph * >::iterator it;
		it = m_bitmapFont->m_fontGlyphs.find( m_commandPromptText[i] );
		if ( it != m_bitmapFont->m_fontGlyphs.end() ) {

			Glyph * glyph = it->second;
			const cbengine::Vector2 & minTexCoords = glyph->getMinTextureCoords();
			const cbengine::Vector2 & maxTexCoords = glyph->getMaxTextureCoords();
			XOffset += m_consoleFontHeight * glyph->getTTFA();
			charWidth = m_consoleFontHeight * glyph->getTTFB();

			// Bottom Left
			textVert.vertexTextureCoords.x = minTexCoords.x;
			textVert.vertexTextureCoords.y = maxTexCoords.y;
			textVert.vertexPosition.x = XOffset;
			textVert.vertexPosition.y = 0.0f;
			textVert.vertexPosition.z = 0.0f;
			m_cPTextVerts.push_back( textVert );
			// Bottom Right
			textVert.vertexTextureCoords.x = maxTexCoords.x;
			textVert.vertexTextureCoords.y = maxTexCoords.y;
			textVert.vertexPosition.x = XOffset + charWidth;
			textVert.vertexPosition.y = 0.0f;
			textVert.vertexPosition.z = 0.0f;
			m_cPTextVerts.push_back( textVert );
			// Top Right
			textVert.vertexTextureCoords.x = maxTexCoords.x;
			textVert.vertexTextureCoords.y = minTexCoords.y;
			textVert.vertexPosition.x = XOffset + charWidth;
			textVert.vertexPosition.y = m_consoleFontHeight;
			textVert.vertexPosition.z = 0.0f;
			m_cPTextVerts.push_back( textVert );
			// Top Left
			textVert.vertexTextureCoords.x = minTexCoords.x;
			textVert.vertexTextureCoords.y = minTexCoords.y;
			textVert.vertexPosition.x = XOffset;
			textVert.vertexPosition.y = m_consoleFontHeight;
			textVert.vertexPosition.z = 0.0f;
			m_cPTextVerts.push_back( textVert );

			XOffset += m_consoleFontHeight * glyph->getTTFC();
			XOffset += charWidth;
			
		} // end if
	} // end for
	
	if ( m_commandPromptText != "" ) {
		cbengine::CBRenderer* sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
		// UNCOMMENTING FIXES MEMORY ERROR?? WHAA
		if ( !m_commandTextVBOIDGenerated ) {

			sharedRenderer->generateVBOBuffers( 1, &m_cpTextVBOID ); 

			m_commandTextVBOIDGenerated = true;
		}
		
		sharedRenderer->bindVBOBuffer( m_cpTextVBOID );
		sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_cPTextVerts.size() ), &m_cPTextVerts.front() );
	}

}


void CommandPrompt::rebuildCursorVBO( float deltaSeconds ) {

	m_cursorVerts.clear();
	const float charWidth = COMMAND_PROMPT_FONT_SIZE * 0.70f;
	const float charHeight = COMMAND_PROMPT_FONT_SIZE;
	static float accumulatedTime = deltaSeconds;
	accumulatedTime += deltaSeconds;
	float sinResult = sin( accumulatedTime * 3.5f );
	float currentAlpha = cbengine::rangeMapFloat( -1.0f, 1.0f, 0.0f, 1.0f, sinResult );
	cbengine::Vertex cursorVert;
	cursorVert.vertexColor.x = cursorColor.x;
	cursorVert.vertexColor.y = cursorColor.y;
	cursorVert.vertexColor.z = cursorColor.z;
	cursorVert.vertexColor.w = currentAlpha;

	std::map< unsigned char, Glyph * >::iterator itStart;
	std::map< unsigned char, Glyph * >::iterator itCursor;

	itCursor = m_bitmapFont->m_fontGlyphs.find( CP_CURSOR_CHAR );

	Glyph * cursorGlyph = itCursor->second;

	const cbengine::Vector2 & minTexCoordsCursorChar = cursorGlyph->getMinTextureCoords();
	const cbengine::Vector2 & maxTexCoordsCursorChar = cursorGlyph->getMaxTextureCoords();
	// Bottom left corner is anchor point
	// Bottom Left
	cursorVert.vertexTextureCoords.x		= minTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= maxTexCoordsCursorChar.y;
	cursorVert.vertexPosition.x = 0.0f;
	cursorVert.vertexPosition.y = 0.0f;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );

	// Bottom Right
	cursorVert.vertexTextureCoords.x		= maxTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= maxTexCoordsCursorChar.y;
	cursorVert.vertexPosition.x = charWidth * 0.10f;
	cursorVert.vertexPosition.y = 0.0f;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );
	// Top Right
	cursorVert.vertexTextureCoords.x		= maxTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= minTexCoordsCursorChar.y;
	cursorVert.vertexPosition.x = charWidth * 0.10f;
	cursorVert.vertexPosition.y = charHeight;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );
	// Top Left
	cursorVert.vertexTextureCoords.x		= minTexCoordsCursorChar.x;
	cursorVert.vertexTextureCoords.y		= minTexCoordsCursorChar.y;
	cursorVert.vertexPosition.x = 0.0f;
	cursorVert.vertexPosition.y = charHeight;
	cursorVert.vertexPosition.z = 0.0f;
	m_cursorVerts.push_back( cursorVert );

	cbengine::CBRenderer * sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	//sharedRenderer->generateVBOBuffers( 1, &m_cursorVBOID );
	sharedRenderer->bindVBOBuffer( m_cursorVBOID );
	sharedRenderer->bufferDataForVBO( ( sizeof( cbengine::Vertex ) * m_cursorVerts.size() ), &m_cursorVerts.front() );

}


void CommandPrompt::registerBuiltInCommands() {
	// Set Commands 
	registerCommand( QUIT_COMMAND_UPPER, quitGame );
	registerCommand( CLEAR_COMMAND_UPPER, clearConsole );
	registerCommand( QUIT_COMMAND_LOWER, quitGame );
	registerCommand( CLEAR_COMMAND_LOWER, clearConsole );
	registerCommand( LOG_COMMAND_UPPER, consoleLog );
	registerCommand( LOG_COMMAND_LOWER, consoleLog );
}


void CommandPrompt::registerCommand( const std::string & commandName, CommandFuncPtr functionCall ) {
	m_commandOptions.insert( std::pair< std::string, CommandFuncPtr >( commandName, functionCall ) );
}


void CommandPrompt::executeConsoleCommand( const std::string & commandName, const std::vector< std::string > & additionalArgs ) {
	UNUSED(commandName);
	ConsoleCommandArg args;
	std::string fullCommand = "";
	for ( size_t i = 0; i < additionalArgs.size(); ++i ) {
		fullCommand += additionalArgs[i];
		fullCommand += ' ';
	}
	args.m_fullCommand = fullCommand;
	args.m_mainCommand = additionalArgs[0];
	args.m_commandArgs = additionalArgs;

	std::map< std::string, CommandFuncPtr >::iterator it;
	for ( it = m_commandOptions.begin(); it != m_commandOptions.end(); ++it ) {

		const std::string & commandKey = it->first;

		if ( commandKey == args.m_mainCommand ) {
			CommandFuncPtr functToCall = it->second;
			ConsoleCommandArg args;
			functToCall( args );
		}
	}
}