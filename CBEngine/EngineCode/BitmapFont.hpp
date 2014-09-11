#ifndef included_BitmapFont
#define included_BitmapFont
#pragma once

#include <string>
#include <map>
#include <cassert>

#include "EngineMacros.hpp"
#include "Texture.hpp"


typedef enum {

	FONT_STATE_UNKNOWN,
	FONT_STATE_NOT_LOADED,
	FONT_STATE_LOADED,
	FONT_STATE_ERROR_XML_FILE_NOT_FOUND,
	FONT_STATE_ERROR_TEXTURE_FILE_NOT_FOUND,
	FONT_STATE_ERROR_XML_AND_TEXTURE_FILES_NOT_FOUND,

} FontState;

class Glyph;

class BitmapFont {
public:

	~BitmapFont();
	explicit BitmapFont( const std::string & fontName, const std::string & fontTextureFilePath, const std::string & fontMetaDataXMLFile );

	FontState loadBitmapFontFromFilesOrCache( const std::string & fontTextureFilePath, const std::string & fontMetaDataXMLFilePath );

	float calculateTextWidth( const std::string & text, float cellHeight );

	// Inline Mutators
	// Setters
	void setBitmapFontTexture( cbengine::Texture * fontTexture );
	void setLocale( const std::string & locale );
	void setNumTextureSheets( unsigned int totalSheets );
	void setCellPixelHeight( float cellPixelHeight );

	// Getters
	const std::string & getFontName() const;
	const std::string & getFontTextureFilePath() const;
	const std::string & getFontMetaDataXMLFilePath() const;
	FontState getFontState() const;

	const std::string & getLocale() const;
	unsigned int getNumTextureSheets() const;
	float getCellPixelHeight() const;

	// Glyphs
	std::map< unsigned char, Glyph * >	m_fontGlyphs;
	cbengine::Texture *			m_texture;

protected:


private:
	PREVENT_COPY_AND_ASSIGN( BitmapFont );

	void setDefaultVariableValues();

	std::string					m_fontName;
	std::string					m_fontTextureFilePath;
	std::string					m_fontMetaDataXMLFilePath;

	FontState					m_fontState;

	// Attributes
	std::string					m_locale;
	unsigned int				m_numSheets;
	float						m_cellPixelHeight;

};

// Inline Mutators
// Setters
inline void BitmapFont::setBitmapFontTexture( cbengine::Texture * fontTexture ) {
	assert( fontTexture != nullptr );
	m_texture = fontTexture;
}


inline void BitmapFont::setLocale( const std::string & locale ) {
	m_locale = locale;
}


inline void BitmapFont::setNumTextureSheets( unsigned int totalSheets ) {
	m_numSheets = totalSheets;
}


inline void BitmapFont::setCellPixelHeight( float cellPixelHeight ) {
	m_cellPixelHeight = cellPixelHeight;
}


// Getters
inline const std::string & BitmapFont::getFontName() const {
	return m_fontName;
}


inline const std::string & BitmapFont::getFontTextureFilePath() const {
	return m_fontTextureFilePath;
}


inline const std::string & BitmapFont::getFontMetaDataXMLFilePath() const {
	return m_fontMetaDataXMLFilePath;
}


inline FontState BitmapFont::getFontState() const {
	return m_fontState;
}


inline const std::string & BitmapFont::getLocale() const {
	return m_locale;
}


inline unsigned int BitmapFont::getNumTextureSheets() const {
	return m_numSheets;
}


inline float BitmapFont::getCellPixelHeight() const {
	return m_cellPixelHeight;
}

#endif