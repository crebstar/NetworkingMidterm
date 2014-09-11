#ifndef included_Console
#define included_Console
#pragma once

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"
#include "Texture.hpp"

// Ugh figure out a fix later
#include "PromptDelegate.hpp"

const float			CONSOLE_WIDTH						= 1600.0f;
const float			CONSOLE_HEIGHT						= 600.0f;

const unsigned int	CONSOLE_MAX_LINES					= 13;
const float			CONSOLE_FONT_HEIGHT					= 30.0f;
const float			CONSOLE_X_MARGIN_OFFSET				= 15.0f;
const float			CONSOLE_Y_LINE_OFFSET				= 20.0f;
const float			CONSOLE_MAX_LINE_WIDTH				= 1400.0f;

// TODO :: Move Console and Command prompt to a debug folder
class  CommandPrompt;
struct ConsoleLine;
class  BitmapFont;

class Console : public PromptDelegate {
public:

	static Console * getSharedDeveloperConsole() {
		static Console developerConsole;
		return &developerConsole;
	}

	~Console();

	void update( float deltaSeconds );
	void render( float deltaSeconds ) const;
	void renderConsoleText( float deltaSeconds ) const;

	// Note: Anchor point is assumed bottom left corner for both Console and CommandPrompt
	void buildConsoleAndCommandPrompt( const std::string & consoleTextureName, 
		const std::string & commandPromptTextureName, 
		const cbengine::Vector4<float> & backgroundColorRGBA,  
		const cbengine::Vector2 & screenPositionCommandPrompt, 
		const cbengine::Vector2 & screenPositionConsole,
		BitmapFont * bitmapFont );

	void addTextToConsole( const std::string & textToAdd, const cbengine::Vector4<float> & color );
	virtual void addStringToConsole( const std::string & stringToAdd, const cbengine::Vector4<float> & color );
	virtual void Console::clearCommandHistory();

	void setConsoleVisible( bool isVisible );
	bool isConsoleVisible() const;

	CommandPrompt *				m_commandPrompt;
	BitmapFont *				m_consoleFont;
	cbengine::Vector2			m_screenPosition;
	cbengine::Vector4<float>	m_backgroundColorRGBA;
	cbengine::Texture *			m_texture;
	float						m_consoleFontHeight;

protected:

private:

	Console() {

		m_commandPrompt = nullptr;
		m_texture = nullptr;
		m_isVisible = false;
	}

	void createVBODataForBackground();

	unsigned int								m_vboBackgroundID;
	std::vector< cbengine::Vertex >				m_backgroundVerts;
	std::vector< ConsoleLine * >				m_consoleLines;
	bool										m_isVisible;
	
};

struct ConsoleLine {
public:
	~ConsoleLine() {}
	explicit ConsoleLine( const cbengine::Vector4<float> & color ) {

		m_lineText = "";
		m_color = color;
		m_lineWidth = 0.0f;
		m_VBOID = 0;
	}


	std::string								m_lineText;
	cbengine::Vector4<float>				m_color;
	std::vector<cbengine::Vertex>			m_verts;
	unsigned int							m_VBOID;
	float									m_lineWidth;

};


inline bool Console::isConsoleVisible() const {

	return m_isVisible;
}


inline void Console::setConsoleVisible( bool isVisible ) {

	m_isVisible = isVisible;
}


#endif