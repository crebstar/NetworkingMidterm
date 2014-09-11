#ifndef included_CommandPrompt
#define included_CommandPrompt
#pragma once

#include <string>
#include <vector>
#include <map>
#include <queue>

#include "..\EngineCode\EngineMacros.hpp"
#include "Vertex.hpp"
#include "Vector2.hpp"
#include "Texture.hpp"
#include "Vector4.hpp"
#include "ConsoleCommandArg.hpp"
#include "CommandFunctions.hpp"

#include "PromptDelegate.hpp"

const float COMMAND_PROMPT_WIDTH		= 1600.0f;
const float COMMAND_PROMPT_HEIGHT		= 50.0f;
const float COMMAND_PROMPT_FONT_SIZE	= 30.0f;

const unsigned char CP_START_LINE_CHAR	= '>';
const unsigned char CP_CURSOR_CHAR		= '|';
const cbengine::Vector2		   textStartPosition( 40.0f, 10.0f );
const cbengine::Vector4<float> cursorColor( 1.0f, 1.0f, 1.0f, 1.0f );
const cbengine::Vector4<float> startLineCharColor( 0.10f, 0.95f, 0.10f, 1.0f );
const cbengine::Vector4<float> cpTextColor( 1.0f, 1.0f, 1.0f, 1.0f );

class BitmapFont;

typedef void (*CommandFuncPtr) (const ConsoleCommandArg & args );

class CommandPrompt {
public:

	~CommandPrompt();
	explicit CommandPrompt( const cbengine::Vector2 & screenPosition, 
		const std::string & backgroundTextureName,
		const cbengine::Vector4<float> & backgroundColorRGBA,
		BitmapFont * bitmapFont );

	void update( float deltaSeconds );
	void render( float deltaSeconds ) const;
	void renderCommandPromptText() const;
	void renderStartLineChar() const;
	void renderCursorChar() const;
	void determineCurrentPositionOfCursor();
	void registerCommand( const std::string & commandName, CommandFuncPtr functionCall );
	void executeConsoleCommand( const std::string & commandName, const std::vector< std::string > & additionalArgs );

	cbengine::Vector2				m_screenPosition;
	cbengine::Vector4<float>		m_backgroundColorRGBA;
	cbengine::Texture*				m_texture;
	BitmapFont*						m_bitmapFont;
	PromptDelegate*					m_promptDelegate;

protected:

private:
	PREVENT_COPY_AND_ASSIGN( CommandPrompt );

	void setUpVertexDataForVBO();
	void setUpVertexDataForStartLineAndCursorVBOS();

	void processKeyboardInput( float deltaSeconds );
	void rebuildCommandPromptStringVBO();
	void rebuildCursorVBO( float deltaSeconds );
	void processCurrentCommandPromptString();
	void insertCharIntoCPString( char charToInsert );
	void registerBuiltInCommands();
	void convertCurrentCommandToCommandArgs( ConsoleCommandArg & args );

	unsigned int								m_vboBackgroundID;
	float										m_consoleFontHeight;
	std::vector< cbengine::Vertex >				m_backgroundVerts;

	cbengine::Vector2							m_cursorCharPosition;
	cbengine::Vector2							m_startCharPosition;
	unsigned int								m_startLineCharVBOID;
	unsigned int								m_cursorVBOID;
	std::vector< cbengine::Vertex >				m_startLineCharVerts;
	std::vector< cbengine::Vertex >				m_cursorVerts;

	std::string									m_commandPromptText;
	std::vector< cbengine::Vertex >				m_cPTextVerts;
	unsigned int								m_cpTextVBOID;
	bool*										m_keysDownLastFrame;
	int											m_indexOffsetFromEndOfCPText;

	std::map< std::string, CommandFuncPtr >		m_commandOptions;
	std::vector< std::string >					m_commandHistory;
	int											m_commandHistoryIndex;

	bool										m_commandTextVBOIDGenerated;
};

// Ignored characters
const unsigned char	CP_IGNORE_TILDE	= 0xc0;

// Built in commands
const std::string QUIT_COMMAND_UPPER			= "QUIT";
const std::string CLEAR_COMMAND_UPPER			= "CLEAR";
const std::string QUIT_COMMAND_LOWER			= "quit";
const std::string CLEAR_COMMAND_LOWER			= "clear";
const std::string LOG_COMMAND_UPPER				= "LOG:";
const std::string LOG_COMMAND_LOWER				= "log:";

#endif