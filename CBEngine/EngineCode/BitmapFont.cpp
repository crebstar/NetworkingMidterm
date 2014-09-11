#include "BitmapFont.hpp"

#include "Glyph.hpp"
#include "TextureManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

BitmapFont::~BitmapFont() {

	std::map< unsigned char, Glyph * >::iterator it;
	for ( it = m_fontGlyphs.begin(); it != m_fontGlyphs.end(); ++it ) {
		delete it->second;
	}

}


BitmapFont::BitmapFont( const std::string & fontName,  const std::string & fontTextureFilePath, const std::string & fontMetaDataXMLFile ) {
	m_fontName = fontName;
	m_fontTextureFilePath = fontTextureFilePath;
	m_fontMetaDataXMLFilePath = fontMetaDataXMLFile;
	setDefaultVariableValues();		
}


FontState BitmapFont::loadBitmapFontFromFilesOrCache( const std::string & fontTextureFilePath, const std::string & fontMetaDataXMLFilePath ) {
	// Not used at the moment
	m_fontTextureFilePath = fontTextureFilePath;
	m_fontMetaDataXMLFilePath = fontMetaDataXMLFilePath;

	// Load font meta data

	// Load font texture

	return m_fontState;
}


void BitmapFont::setDefaultVariableValues() {
	m_texture = nullptr;
	m_fontState = FONT_STATE_NOT_LOADED;
}


float BitmapFont::calculateTextWidth( const std::string & text, float cellHeight ) {

	float totalTextWidth = 0.0f;
	for ( size_t i = 0; i < text.size(); ++i ) {

		std::map< unsigned char, Glyph * >::iterator it;
		it = m_fontGlyphs.find( text[i] );
		
		if ( it != m_fontGlyphs.end() ) {
			Glyph * glyph = it->second;
			totalTextWidth += cellHeight * glyph->getTTFA();
			totalTextWidth += cellHeight * glyph->getTTFB();
			totalTextWidth += cellHeight * glyph->getTTFC();
		}
	}

	return totalTextWidth;
}