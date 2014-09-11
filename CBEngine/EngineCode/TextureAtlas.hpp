#ifndef included_TextureAtlas
#define included_TextureAtlas
#pragma once

#include <map>
#include <string>

#include "EngineMacros.hpp"
#include "Vector2.hpp"
#include "Size.hpp"
#include "Texture.hpp"

class Texture;

class TextureAtlas {
public:
	~TextureAtlas();
	explicit TextureAtlas( const std::string& textureAtlasFilePath, const std::string& textureAtlasFileName, bool loadTextureAtlas = true );

	// Function does not modify the texCoords if the entry is not found
	void getTextureCoordsForImageRef( const std::string& imageRef, cbengine::Vector2& minTexCoords, cbengine::Vector2& maxTexCoords );

	// Scoped Class
	struct SpriteData {
	public:
		explicit SpriteData( const std::string& spriteFileName, const cbengine::Vector2& minTexCoords, const cbengine::Vector2& maxTexcoords, const cbengine::Size& heightWidth, const bool texCoordsNormalized ) :
		m_spriteFileName( spriteFileName ),
		m_minTextureCoords( minTexCoords ),
		m_maxTextureCoords( maxTexcoords ),
		m_heightWidth( heightWidth ),
		m_textureCoordinatesNormalized( texCoordsNormalized )
		{

		}

		std::string					m_spriteFileName;
		cbengine::Vector2			m_minTextureCoords;
		cbengine::Vector2			m_maxTextureCoords;
		cbengine::Size				m_heightWidth;
		bool						m_textureCoordinatesNormalized;
	};

	// Core Functions
	bool loadTextureAtlasFromXMLFile();
protected:

private:
	PREVENT_COPY_AND_ASSIGN( TextureAtlas );

	std::string										m_textureAtlasFileName;
	std::string										m_textureAtlasFilePath;
	std::map<std::string,SpriteData>				m_spriteDataMap;
	cbengine::Texture*								m_textureAtlasTexture;
	cbengine::Size									m_heightWidth;
	mutable bool									m_textureAtlasLoaded;
};


inline void TextureAtlas::getTextureCoordsForImageRef( const std::string& imageRef, cbengine::Vector2& minTexCoords, cbengine::Vector2& maxTexCoords ) {
	std::map<std::string,SpriteData>::iterator it;
	it = m_spriteDataMap.find( imageRef );
	if ( it != m_spriteDataMap.end() ) {
		TextureAtlas::SpriteData& spriteData = it->second;
		minTexCoords.x = spriteData.m_minTextureCoords.x;
		minTexCoords.y = spriteData.m_minTextureCoords.y;
		maxTexCoords.x = spriteData.m_maxTextureCoords.x;
		maxTexCoords.y = spriteData.m_maxTextureCoords.y;
	}
}


namespace TextureAtlasConstants {
// TEXTUREPACKER XML NAMES
const std::string		TEXTURE_ATLAS_ROOT_NODE_NAME					 = "TextureAtlas";
const std::string		TEXTURE_ATLAS_TEXTURE_IMAGE_PATH_ATTRIB_NAME	 = "imagePath";
const std::string		TEXTURE_ATLAS_TEXTURE_HEIGHT_ATTRIB_NAME		 = "height";
const std::string		TEXTURE_ATLAS_TEXTURE_WIDTH_ATTRIB_NAME			 = "width";
const std::string		TEXTURE_ATLAS_SPRITE_DATA_NODE_NAME				 = "sprite";
const std::string		TEXTURE_ATLAS_SPRITE_NAME_ATTRIB_NAME			 = "n";
const std::string       TEXTURE_ATLAS_SPRITE_XCOORD_ATTRIB_NAME			 = "x";
const std::string		TEXTURE_ATLAS_SPRITE_YCOORD_ATTRIB_NAME			 = "y";
const std::string		TEXTURE_ATLAS_SPRITE_HEIGHT_ATTRIB_NAME			 = "h";
const std::string		TEXTURE_ATLAS_SPRITE_WIDTH_ATTRIB_NAME			 = "w";

}

#endif