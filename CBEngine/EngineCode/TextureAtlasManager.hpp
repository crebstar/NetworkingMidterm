#ifndef included_TextureAtlasManager
#define included_TextureAtlasManager
#pragma once

#include <string>
#include <map>

#include "EngineMacros.hpp"
#include "TextureAtlas.hpp"

class TextureAtlasManager {
public:
	static TextureAtlasManager& getSharedTextureAtlasManager() {
		static TextureAtlasManager textureAtlasManager;
		return textureAtlasManager;
	}

	~TextureAtlasManager();

	// Inline Functions
	TextureAtlas * loadTextureAtlasOrGrabFromCache( const std::string& textureAtlasXMLFilePath, const std::string& textureAtlasXMLFileName );
	TextureAtlas * getAtlasFromCache( const std::string& textureAtlasFileName );

	//Inline Mutators
protected:
	explicit TextureAtlasManager() {

	}


private:
	PREVENT_COPY_AND_ASSIGN( TextureAtlasManager );

	std::map<std::string,TextureAtlas*>			m_textureAtlasMap;
};


// Inline Functions
inline TextureAtlas * TextureAtlasManager::loadTextureAtlasOrGrabFromCache( const std::string& textureAtlasXMLFilePath, const std::string& textureAtlasXMLFileName ) {
	std::map<std::string,TextureAtlas*>::iterator it;
	it = m_textureAtlasMap.find( textureAtlasXMLFileName );
	if ( it != m_textureAtlasMap.end() ) {
		// TextureAtlas has already been loaded
		return it->second;
	}

	TextureAtlas * atlasToAdd = new TextureAtlas( textureAtlasXMLFilePath, textureAtlasXMLFileName, true );
	m_textureAtlasMap.insert( std::pair<std::string,TextureAtlas*>( textureAtlasXMLFileName, atlasToAdd ) );
	return atlasToAdd;
}


inline TextureAtlas * TextureAtlasManager::getAtlasFromCache( const std::string& textureAtlasFileName ) {
	std::map<std::string,TextureAtlas*>::iterator it;
	it = m_textureAtlasMap.find( textureAtlasFileName );
	if ( it != m_textureAtlasMap.end() ) {
		return it->second;
	} 
	return nullptr;
}

#endif