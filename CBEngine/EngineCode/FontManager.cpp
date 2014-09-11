#include "FontManager.hpp"

#include <cassert>

#include "BitmapFont.hpp"
#include "TextureManager.hpp"
#include "Glyph.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

FontManager::~FontManager() {
	// Free the cached fonts
	std::map< std::string, BitmapFont *>::iterator it;
	for ( it = m_bitmapFontCache.begin(); it != m_bitmapFontCache.end(); ++it ) {
		BitmapFont * fontToDelete = it->second;
		delete fontToDelete;
	}
}


BitmapFont * FontManager::loadBitmapFont( const std::string & fontName, const std::string & fontXMLMetaDataFileName, const std::string & fontTextureFilePath ) {
	// First check to see if the font is loaded or not
	BitmapFont * bitmapFont = nullptr;
	std::map< std::string, BitmapFont * >::iterator itFont;
	itFont = m_bitmapFontCache.find( fontName );

	if ( itFont != m_bitmapFontCache.end() ) {
		bitmapFont = itFont->second;
		return bitmapFont;
	} else {
		bitmapFont = new BitmapFont( fontName, fontTextureFilePath, fontXMLMetaDataFileName );
		m_bitmapFontCache.insert( std::pair<std::string,BitmapFont*>( fontName, bitmapFont ) );
	}

	// Generate the texture or grab it from the cache
	cbengine::Texture * bitmapFontTexture = createFontTexture( fontTextureFilePath );
	bitmapFont->setBitmapFontTexture( bitmapFontTexture );

	// Load the Glyph meta Data
	loadBitmapFontGlyphMetaDataFromXMLFile( fontXMLMetaDataFileName, *(bitmapFont) );

	return bitmapFont;

}


cbengine::Texture * FontManager::createFontTexture( const std::string & fontTextureName ) {

	cbengine::Texture * fontTexture = nullptr;
	cbengine::TextureManager * sharedTextureManager = cbengine::TextureManager::sharedTextureManager();
	fontTexture = sharedTextureManager->generateOrGrabExistingTexture( fontTextureName );
	assert( fontTexture != nullptr );

	return fontTexture;
}


void FontManager::loadBitmapFontGlyphMetaDataFromXMLFile( const std::string & fontXMLMetaDataFileName, BitmapFont & bitmapFont ) {

	pugi::xml_document bitmapFontXMLDocument;
	bitmapFontXMLDocument.load_file( fontXMLMetaDataFileName.c_str() );

	pugi::xml_node rootNode			= bitmapFontXMLDocument.child( BITMAP_FONT_ROOT_NODE.c_str() );
	pugi::xml_node fontInfoNode		= rootNode.child( BITMAP_FONTINFO_NODE.c_str() );
	UNUSED(rootNode);

	// Extract attributes from the fontInfoNode
	pugi::xml_attribute localeAttribute				= fontInfoNode.attribute( BITMAP_FONTINFO_ATTRIBUTE_LOCALE.c_str() );
	pugi::xml_attribute numTextureSheetsAttribute	= fontInfoNode.attribute( BITMAP_FONTINFO_ATTRIBUTE_NUMTEXTURESHEETS.c_str() ); 
	pugi::xml_attribute cellPixelHeightAttribute	= fontInfoNode.attribute( BITMAP_FONTINFO_ATTRIBUTE_CELLPIXELHEIGHT.c_str() );

	std::string localeAsString = localeAttribute.as_string();
	unsigned int numTexturesheetsAsUint = numTextureSheetsAttribute.as_uint();
	float cellPixelHeightAsFloat = cellPixelHeightAttribute.as_float();
	bitmapFont.setLocale( localeAsString );
	bitmapFont.setNumTextureSheets( numTexturesheetsAsUint );
	bitmapFont.setCellPixelHeight( cellPixelHeightAsFloat );

	// Load Glyph Data
	Glyph * fontGlyph = nullptr;
	pugi::xml_node::iterator itNode;
	for ( itNode = fontInfoNode.begin(); itNode != fontInfoNode.end(); ++itNode ) {
		
		fontGlyph = new Glyph;
		pugi::xml_attribute ucsIndexAtt		= itNode->attribute( BITMAP_GLYPH_ATTRIBUTE_UCSINDEX.c_str() );
		pugi::xml_attribute sheetAtt		= itNode->attribute( BITMAP_GLYPH_ATTRIBUTE_SHEET.c_str() );
		pugi::xml_attribute texCoordMinsAtt	= itNode->attribute( BITMAP_GLYPH_ATTRIBUTE_TEXCOORDMINS.c_str() );
		pugi::xml_attribute texCoordMaxsAtt	= itNode->attribute( BITMAP_GLYPH_ATTRIBUTE_TEXCOORDMAXS.c_str() );
		pugi::xml_attribute ttfAAtt			= itNode->attribute( BITMAP_GLYPH_ATTRIBUTE_TTFA.c_str() );
		pugi::xml_attribute ttfBtt			= itNode->attribute( BITMAP_GLYPH_ATTRIBUTE_TTFB.c_str() );
		pugi::xml_attribute ttfCtt			= itNode->attribute( BITMAP_GLYPH_ATTRIBUTE_TTFC.c_str() );

		int ucsAsInt = ucsIndexAtt.as_int();
		fontGlyph->setUCSIndex( static_cast< unsigned char>( ucsAsInt ) );
		fontGlyph->setTTFA( ttfAAtt.as_float() );
		fontGlyph->setTTFB( ttfBtt.as_float() );
		fontGlyph->setTTFC( ttfCtt.as_float() );
		fontGlyph->setSheetNum( sheetAtt.as_uint() );

		// Must split the texture coords
		std::string minTextureAsString = texCoordMinsAtt.as_string();
		std::string maxTextureAsString = texCoordMaxsAtt.as_string();
		std::string xMinTC;
		std::string yMinTC;
		std::string xMaxTC;
		std::string yMaxTC;

		bool commaDelimFound = false;
		for ( size_t i = 0; i < minTextureAsString.size(); ++i ) {

			if ( minTextureAsString[i] == BITMAP_GLYPH_TEXCOORD_DELIMITER ) {
				commaDelimFound = true;
				continue;
			}

			if ( !commaDelimFound ) {
				xMinTC += minTextureAsString[i];
			} else {
				yMinTC += minTextureAsString[i];
			}
			
		}

		cbengine::Vector2 minTexCoords( std::stof( xMinTC), std::stof( yMinTC ) );
		fontGlyph->setMinTextureCoords( minTexCoords );

		commaDelimFound = false;
		for ( size_t i = 0; i < maxTextureAsString.size(); ++i ) {

			if ( maxTextureAsString[i] == BITMAP_GLYPH_TEXCOORD_DELIMITER ) {
				commaDelimFound = true;
				continue;
			}

			if ( !commaDelimFound ) {
				xMaxTC += maxTextureAsString[i];
			} else {
				yMaxTC += maxTextureAsString[i];
			}

		}

		cbengine::Vector2 maxTexCoords( std::stof( xMaxTC ), std::stof( yMaxTC ) );
		fontGlyph->setMaxTextureCoords( maxTexCoords );

		bitmapFont.m_fontGlyphs.insert( std::pair< unsigned char, Glyph *>( fontGlyph->getUCSIndex(), fontGlyph) );

	} // end for

} // end function