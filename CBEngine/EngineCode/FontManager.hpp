
#ifndef included_FontManager
#define included_FontManager
#pragma once

#include <map>
#include <string>

#include "EngineMacros.hpp"
#include "pugixml.hpp"
#include "Texture.hpp"

class BitmapFont;


class FontManager {
public:

	static FontManager * sharedFontManager() {
		static FontManager sharedFontManager;
		return &sharedFontManager;
	}

	~FontManager();

	BitmapFont * loadBitmapFont( const std::string & fontName, const std::string & fontXMLMetaDataFileName, const std::string & fontTextureFilePath );

protected:


private:
	PREVENT_COPY_AND_ASSIGN( FontManager );

	FontManager() {}

	cbengine::Texture * createFontTexture( const std::string & fontTextureName );
	void loadBitmapFontGlyphMetaDataFromXMLFile( const std::string & fontXMLMetaDataFileName, BitmapFont & bitmapFont );

	// Instance Variables
	// Map which caches loaded fonts
	std::map< std::string, BitmapFont* >			m_bitmapFontCache;

};

//-----------------GLYPH XML NODES AND NODE ATTRIBUTE CONSTANTS----------------------//
// ROOT NODE
const std::string BITMAP_FONT_ROOT_NODE							= "FontDefinition";
// FIRST CHILD
const std::string BITMAP_FONTINFO_NODE							= "FontInfo";
const std::string BITMAP_FONTINFO_ATTRIBUTE_NAME				= "name";
const std::string BITMAP_FONTINFO_ATTRIBUTE_LOCALE				= "locale";
const std::string BITMAP_FONTINFO_ATTRIBUTE_NUMTEXTURESHEETS	= "numTextureSheets";
const std::string BITMAP_FONTINFO_ATTRIBUTE_CELLPIXELHEIGHT		= "cellPixelHeight";
// SECOND CHILD GLYPH
const std::string BITMAP_GLYPH_NODE								= "Glyph";
const std::string BITMAP_GLYPH_ATTRIBUTE_UCSINDEX				= "ucsIndex";
const std::string BITMAP_GLYPH_ATTRIBUTE_SHEET					= "sheet";
const std::string BITMAP_GLYPH_ATTRIBUTE_TEXCOORDMINS			= "texCoordMins";
const std::string BITMAP_GLYPH_ATTRIBUTE_TEXCOORDMAXS			= "texCoordMaxs";
const std::string BITMAP_GLYPH_ATTRIBUTE_TTFA					= "ttfA";
const std::string BITMAP_GLYPH_ATTRIBUTE_TTFB					= "ttfB";
const std::string BITMAP_GLYPH_ATTRIBUTE_TTFC					= "ttfC";
const char  BITMAP_GLYPH_TEXCOORD_DELIMITER						= ',';
//----------------------------------------------------------------------------------//


#endif