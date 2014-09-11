
#ifndef included_TextureManager
#define included_TextureManager
#pragma once

#include <map>
#include <string>

#include "EngineMacros.hpp"

namespace cbengine {

class Texture;
class TextureManager {

public:

	// Static Methods
	static TextureManager * sharedTextureManager() {

		static TextureManager textureManager;
		return &textureManager;

	} // end sharedTextureManager

	// Dtor
	virtual ~TextureManager();

	// Core Functions
	virtual Texture * generateOrGrabExistingTexture( const std::string & textureFileName );

	// Predicates
	virtual inline bool isTextureCached( const std::string & textureFileName ) {
		
		std::map< std::string , Texture * >::iterator it = m_textureCache.find( textureFileName );

		return (it != m_textureCache.end());

	} // end isTextureCached

	PREVENT_COPY_AND_ASSIGN(TextureManager);
protected:

private:

	// Prevent instantiation outside header file with private Ctor
	TextureManager() {}

	// Private Functions
	virtual Texture * generateTextureDataFromFile( const std::string & textureFileName );
	virtual void cacheTexture( const std::string & textureName, Texture * textureToCache );

	// Instance Variables
	std::map< std::string , Texture *>		m_textureCache;

};

} // end namespace

#endif